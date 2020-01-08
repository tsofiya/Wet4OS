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
    List();

    /**
     * Destructor
     */
    ~List();

    class Node {
    public:
        T data;
        Node *next;
        //Node *prev;
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
    bool insert(const T &data);

    /**
     * Remove the node that its data equals to @param value
     * @param value the data to lookup a node that has the same data to be removed
     * @return true if a matched node was found and removed and false otherwise
     */
    bool remove(const T &value);

    /**
     * Returns the current size of the list
     * @return the list size
     */
    unsigned int getSize();

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


    void updateSize(bool increase);
    // TODO: Add your own methods and data members
};

#endif //THREAD_SAFE_LIST_H_