#include <iostream>
#include <stack>
#include <mutex>
#include <thread>
#include <atomic>
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <unistd.h>

using namespace std;

#define PUSH 1
#define POP 2


/* This file contains implementation of various stacks and queues*/
class Node {
public:
    std::atomic<Node*> next;
    std::atomic<bool> locked;
    Node() : locked(false), next(nullptr) {}
};

class MCSLock {
public:
    MCSLock() {
        tail_.store(nullptr, std::memory_order_relaxed);
    }

    bool lock(Node* node) {
        Node* prev = tail_.load(std::memory_order_seq_cst);
        node->next.store(nullptr, std::memory_order_relaxed);
        if(! tail_.compare_exchange_weak(prev,node, std::memory_order_seq_cst)){
            return 0;
        }

        if (prev != nullptr) {
            node->locked.store(true, std::memory_order_relaxed);
            prev->next.store(node, std::memory_order_seq_cst);

            while (node->locked.load(std::memory_order_seq_cst)) {}
        }
        return 1;
    }
    void unlock(Node* node) {
        Node* m = node;

                if (tail_.compare_exchange_strong(m, nullptr, std::memory_order_seq_cst)) {
                    return;
                }
                while (node->next.load(std::memory_order_seq_cst) == nullptr) {}
            
            node->next.load(std::memory_order_seq_cst)->locked.store(false, std::memory_order_seq_cst);
    }

private:
    std::atomic<Node*> tail_;
};

class MichaelScottQueue {
private:
    struct Node {
        int data;
        std::atomic<Node*> next;

        Node(int value) : data(value), next(nullptr) {}
    };

    alignas(64) std::atomic<Node*> head;
    alignas(64) std::atomic<Node*> tail;

public:
    MichaelScottQueue() {
        Node* dummyNode = new Node(0);
        head.store(dummyNode);
        tail.store(dummyNode);
    }

    ~MichaelScottQueue() {
        while (Node* oldHead = head.load()) {
            head.store(oldHead->next);
            delete oldHead;
        }
    }

    void enqueue(int value) {
        Node* newNode = new Node(value);
        Node* t, *e;
        while(true) {
        t = tail.load();
        e = t->next.load();
        if (e == NULL && t->next.compare_exchange_strong(e, newNode, std::memory_order_seq_cst)){break;}
        else if(e!=NULL){tail.compare_exchange_strong(t,e,std::memory_order_seq_cst );}
        }
    }

    bool dequeue(int& value) {
        Node *t, *h, *n;
        while(true){
            h=head.load();
            t=tail.load();
            n=h->next.load();
            if(h==head.load()){
                if(h==t){
                    if(n==NULL){return NULL;}
                    else{tail.compare_exchange_strong(t,n,std::memory_order_seq_cst);}
                    }
                else{
                int ret=n->data;
                if(head.compare_exchange_strong(h,n,std::memory_order_seq_cst)){return ret;}
                }
            }
        }
    }

    bool isEmpty() const {
        return head.load()->next.load() == nullptr;
    }
};

// Implementation Eleminations Array used in stacks
class EliminationArray {
private:
    static const int kCapacity = 5;

    struct Slot {
        std::atomic<int> value;
        std::atomic<bool> isOccupied;
        std::atomic<int> operation;

        Slot() : value(0), operation(0), isOccupied(false) {}
    };

    std::vector<Slot> slots;

public:
    EliminationArray() : slots(kCapacity) {}

    int visit(int& value, int operation) {
        int index = rand() % kCapacity;
        Slot& slot = slots[index];
        int tmp;

        if (slot.isOccupied.load()){
            if (operation == slot.operation.load()){ // If the same type
                return 2;
            }
            else {
            tmp = slot.value.load();
            store(index, value, operation, true);
            
            value = tmp;
            return 1; // Eliminated
            }
        }
        store(index, value, operation, false);
        usleep(15000); //sleep for 5 miliseconds and recheck
        if (slot.value.load() == value && slot.operation.load()== operation){
            //slot.isOccupied.store(false);
             return 2;
        }
        return 0; // No elimination
    }

    void store(int index, int value, int operation, bool occ) {
        Slot& slot = slots[index];

        slot.value.store(value);
        slot.operation.store(operation);
        if (occ) {
        slot.isOccupied.store(true);
        }
    }


};

class TreiberStackWithElimination {
private:
    struct Node {
        int data;
        Node* next;

        Node(int value) : data(value), next(nullptr) {}
    };

    std::atomic<Node*> top = NULL;
    EliminationArray eliminationArray;

public:
    TreiberStackWithElimination() : top(nullptr) {}

    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = top.load();
        int chiz;

        if (!top.compare_exchange_strong(newNode->next, newNode)) {
            int result = eliminationArray.visit(newNode->data, PUSH);
            while (result != 2){
                result = eliminationArray.visit(newNode->data, PUSH);
            }
            if (result == 1){
                pop(chiz);
                return;
            }
        }
        return;
    }

    bool pop(int& value) {
        Node* oldTop = top.load();
        while (oldTop) {
            if (top.compare_exchange_strong(oldTop, oldTop->next)) {
                value = oldTop->data;
                delete(oldTop);
                return true;
            }
            int result = eliminationArray.visit(value,POP);
            while (result != 2){
                result = eliminationArray.visit(value,POP);
            }
            if (result == 1) {
                push(value);
                return true;
            }
        }
        return false;
    }

    bool isEmpty() const {
        return top.load() == NULL;
    }
};


class SGLStack {
private:
    stack<int> stack_;
    MCSLock mcsLock;
    mutex lkmutex;

public:
    bool push(int value) {
        Node node;
        int chiz;
        EliminationArray eliminationArray;

        if (mcsLock.lock(&node)){
        stack_.push(value);
        mcsLock.unlock(&node);
        return true;
        }
        int result = eliminationArray.visit(value, PUSH);
        while (result != 2){
            result = eliminationArray.visit(value, PUSH);
        }
        if (result == 1){
        pop(chiz);
        return false;
        }
        
    }

    bool pop(int &value) {
        Node node;
        int chiz;
        EliminationArray eliminationArray;

        if (mcsLock.lock(&node)){
        if (!stack_.empty()) {
            value = stack_.top();
            stack_.pop();
            mcsLock.unlock(&node);
            return true;
        }
        }
        int result = eliminationArray.visit(value, POP);
        while (result != 2){
            result = eliminationArray.visit(value, POP);
        }
        if (result == 1){
        push(value);
        return false;
        }
        return false;
    }

    bool isEmpty() {
        lkmutex.lock();
        bool res = stack_.empty();
        lkmutex.unlock();
        return res;
    }
};


class SGLQueue {
private:
    queue<int> queue_;
    mutex lkmutex;

public:
    void enqueue(int value) {
        lock_guard<mutex> lock(lkmutex);
        queue_.push(value);
    }

    bool dequeue(int& value) {
        lock_guard<mutex> lock(lkmutex);
        if (!queue_.empty()) {
            value = queue_.front();
            queue_.pop();
            return true;
        }
        return false;
    }

    bool isEmpty() {
        lkmutex.lock();
        bool res = queue_.empty();
        lkmutex.unlock();
        return res;
    }
};

