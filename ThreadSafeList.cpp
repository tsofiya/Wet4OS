//
// Created by student on 1/8/20.
//

#include "ThreadSafeList.h"

template<typename T>
List<T>::List() { //TODO: add your implementation
    pthread_mutex_init(&counterLock, NULL);
    pthread_mutex_init(&headLock, NULL);
}

template<typename T>
List<T>::~List() {
    pthread_mutex_lock(&headLock);
    deleted= true;
    pthread_mutex_destroy(&counterLock);
    if (head==NULL) {
        pthread_mutex_unlock(&headLock);
        pthread_mutex_destroy(&headLock);
    }else {
        Node *ptr = head;
        while (ptr != NULL) {
            Node *forDel = ptr;
            ptr = ptr->next;
            delete (forDel);
        }
    }
}

template<typename T>
bool List<T>::insert(const T &data){
    //TODO: add your implementation
    if (deleted)
        return false;
    pthread_mutex_lock(&headLock);
    if (head==NULL){
        head= new Node(data);
        updateSize(true);
        pthread_mutex_unlock(&headLock);
        return true;

    }

    pthread_mutex_t prevLock=headLock;
    pthread_mutex_t currLock;
    pthread_mutex_lock(&currLock);
    Node* ptr= head;
    while (ptr->next != NULL && ptr->next->data< data){
        ptr=ptr->next;
        pthread_mutex_unlock(&prevLock);
        prevLock=currLock;
        currLock=ptr->nodeLock;
        pthread_mutex_lock(&currLock);
    }

    if (ptr->data==data){
        pthread_mutex_unlock(&prevLock);
        pthread_mutex_unlock(&currLock);
        return false;
    }

    Node * newNode= new Node(data, ptr->next);
    ptr->next=newNode;
    updateSize(true);
    pthread_mutex_unlock(&prevLock);
    pthread_mutex_unlock(&currLock);
    return true;
}

template<typename T>
unsigned int List<T>:: getSize() {
    //TODO: add your implementation
    pthread_mutex_lock(&counterLock);
    int currCount = counter;
    pthread_mutex_unlock(&counterLock);
    return currCount;

}
template<typename T>
bool List<T>::remove(const T &value){
    if (deleted)
        return false;
    pthread_mutex_lock(&headLock);
    Node*ptr=head;
    if (head==NULL){
        pthread_mutex_unlock(&headLock);
        return false;
    }

    pthread_mutex_t prevLock=headLock;
    pthread_mutex_t currLock=head->nodeLock;
    if (head->data==value){
        Node* temp=head;
        head=head->next;
        updateSize(false);
        pthread_mutex_unlock(&prevLock);
        pthread_mutex_unlock(&currLock);
        delete(temp);

        return true;

    }

    while(ptr->next!=NULL){
        if (ptr->next->data==value){
            Node* temp=ptr->next;
            ptr->next=ptr->next->next;
            updateSize(false);
            pthread_mutex_unlock(&prevLock);
            pthread_mutex_unlock(&currLock);
            delete(temp);

            return true;
        }

        ptr=ptr->next;
    }

    return false;


}

template<typename T>
void List<T>::updateSize(bool increase){
    pthread_mutex_lock(&counterLock);
    if (increase)
        counter++;
    else
        counter--;
    pthread_mutex_unlock(&counterLock);
}