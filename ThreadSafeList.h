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
    ~List() { //TODO: add your implementation
        pthread_mutex_destroy(&counterLock);
        Node* ptr= head;
        while (ptr!=NULL){
            Node *forDel= ptr;
            ptr=ptr->next;
            delete(forDel);
        }

    }

    class Node {
    public:
        T data;
        Node *next;
        Node *prev;
        pthread_mutex_t nodeLock;

        Node(const T &data, Node*prev=NULL, Node* next=NULL):data(data), prev(prev), next(next){
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
    bool insert(const T &data) {
        //TODO: add your implementation

        Node * newNode= new Node(data);

        bool success= false;
        Node* ptr= head;
        while (!success) {
            if (head==NULL){
                pthread_mutex_lock(&headLock);
                if (head==NULL) {
                    head= newNode;
                    pthread_mutex_unlock(&headLock);
                    return true;
                }
                pthread_mutex_unlock(&headLock);
            }


            while (ptr->next != NULL && ptr->data> data) {
                ptr=ptr->next;

            }

            //Lock previous node
            int attemptPrev=1;
            if(ptr->prev!=NULL)
                attemptPrev=pthread_mutex_trylock(ptr->nodeLock);

            //Lock curr Node
            int attempt=attempt= pthread_mutex_trylock(ptr->nodeLock);

            //if locking succesful, exit loop.
            if ((attemptPrev==0 && attempt==0) ||(attempt==0 && ptr->prev==NULL))
                success= true;

            //otherwise, try again.
            else if (attemptPrev==0){
                pthread_mutex_unlock(ptr->prev->nodeLock);
            }else if (attempt==0){
                pthread_mutex_unlock(ptr->nodeLock);
            }
        }

        if (ptr->prev==NULL){
            head=newNode;
            newNode->next=ptr;
            ptr->prev= newNode;
            pthread_mutex_unlock(ptr->nodeLock);
        }else{
            newNode->prev=ptr->prev;
            newNode->prev->next= newNode;
            newNode->next=ptr;
            ptr->prev=newNode;
            pthread_mutex_unlock(ptr->nodeLock);
            pthread_mutex_unlock(newNode->prev->nodeLock);
        }


    }

    /**
     * Remove the node that its data equals to @param value
     * @param value the data to lookup a node that has the same data to be removed
     * @return true if a matched node was found and removed and false otherwise
     */
    bool remove(const T &value) {
        //TODO: add your implementation
    }

    /**
     * Returns the current size of the list
     * @return the list size
     */
    unsigned int getSize() {
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
    pthread_mutex_t counterLock;
    pthread_mutex_t headLock;

    // TODO: Add your own methods and data members
};

#endif //THREAD_SAFE_LIST_H_