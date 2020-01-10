#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include <pthread.h>
#include <iostream>
#include <iomanip> // std::setw
#include <pthread.h>

using namespace std;

template<typename T>
class List {

public:
    /**
     * Constructor
     */
    List() { //TODO: add your implementation
        pthread_mutex_init(&counterLock, NULL);
        pthread_mutex_init(&headLock, NULL);
    }

    /**
     * Destructor
     */
    ~List(){
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

    class Node {
    public:
        T data;
        Node *next;
        pthread_mutex_t nodeLock;

        Node(const T &data, Node* next=NULL):data(data), next(next){
            pthread_mutex_init(&nodeLock, NULL);
        }

        ~Node(){
            pthread_mutex_destroy(&nodeLock);
        }

        // TODO: Add your methods and data members
    };

    /**
     * Insert new node to list while keeping the list ordered in an ascending order
     * If there is already a node has the same data as @param data then return false (without adding it again)
     * @param data the new data to be added to the list
     * @return true if a new node was added and false otherwise
     */
    bool insert(const T &data){
        {
            //TODO: add your implementation
            if (deleted)
                return false;
            pthread_mutex_lock(&headLock);
            if (head==NULL){
                head= new Node(data);
                updateSize(true);
                __add_hook();
                pthread_mutex_unlock(&headLock);
                return true;

            }

            pthread_mutex_t *prevLock=&headLock;
            pthread_mutex_t *currLock=&(head->nodeLock);
            pthread_mutex_lock(currLock);
            Node* ptr= head;
            while (ptr->next != NULL && ptr->next->data< data){
                ptr=ptr->next;
                pthread_mutex_unlock(prevLock);
                prevLock=currLock;
                currLock=&(ptr->nodeLock);
                pthread_mutex_lock(currLock);
            }

            if (ptr->data==data){
                pthread_mutex_unlock(prevLock);
                pthread_mutex_unlock(currLock);
                return false;
            }

            Node * newNode= new Node(data, ptr->next);
            ptr->next=newNode;
            updateSize(true);
            __add_hook();
            pthread_mutex_unlock(prevLock);
            pthread_mutex_unlock(currLock);
            return true;
        }
    }

    /**
     * Remove the node that its data equals to @param value
     * @param value the data to lookup a node that has the same data to be removed
     * @return true if a matched node was found and removed and false otherwise
     */
    bool remove(const T &value){
        if (deleted)
            return false;
        pthread_mutex_lock(&headLock);
        Node*ptr=head;
        if (head==NULL){
            pthread_mutex_unlock(&headLock);
            return false;
        }

        pthread_mutex_t *prevLock=&headLock;
        pthread_mutex_t *currLock=&(head->nodeLock);
        if (head->data==value){
            Node* temp=head;
            head=head->next;
            updateSize(false);
            pthread_mutex_unlock(prevLock);
            pthread_mutex_unlock(currLock);
            delete(temp);

            return true;

        }

        while(ptr->next!=NULL){
            if (ptr->next->data==value){
                Node* temp=ptr->next;
                ptr->next=ptr->next->next;
                updateSize(false);
                __remove_hook();
                pthread_mutex_unlock(prevLock);
                pthread_mutex_unlock(currLock);
                delete(temp);

                return true;
            }

            ptr=ptr->next;
        }

        return false;


    }

    /**
     * Returns the current size of the list
     * @return the list size
     */
    unsigned int getSize(){
        //TODO: add your implementation
        pthread_mutex_lock(&counterLock);
        int currCount = counter;
        pthread_mutex_unlock(&counterLock);
        return currCount;

    }

    // Don't remove
    void print() {
        pthread_mutex_lock(&list_mutex);
        Node *temp = head;
        if (temp == NULL) {
            cout << "";
        } else if (temp->next == NULL) {
            cout << temp->data;
        } else {
            while (temp != NULL) {
                cout << right << setw(3) << temp->data;
                temp = temp->next;
                cout << " ";
            }
        }
        cout << endl;
        pthread_mutex_unlock(&list_mutex);
    }


    // Don't remove
    virtual void __add_hook() {}

    // Don't remove
    virtual void __remove_hook() {}

private:
    Node *head;
    int counter = 0;
    bool deleted=false;
    pthread_mutex_t counterLock;
    pthread_mutex_t headLock;
    pthread_mutex_t list_mutex;


    void updateSize(bool increase){
        pthread_mutex_lock(&counterLock);
        if (increase)
            counter++;
        else
            counter--;
        pthread_mutex_unlock(&counterLock);
    }
    // TODO: Add your own methods and data members
};

#endif //THREAD_SAFE_LIST_H_