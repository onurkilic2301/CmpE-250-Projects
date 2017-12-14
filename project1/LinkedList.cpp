#include "LinkedList.h"

LinkedList::LinkedList(){
    this->length = 0;
};

LinkedList::LinkedList(const LinkedList &list){
    this->length = list.length;
    if(list.head){
        this->head = new Node(*(list.head));
    }
    //Iterating to find the new tail of the list
    if(list.tail) {
        Node *temp = this->head;
        for (int i = 0; i < length-1 ; i++)
            temp = temp->next;
        this->tail = temp;
    }
};

LinkedList &LinkedList::operator=(const LinkedList &list){
    this->length = list.length;
    if(this->head) {
        delete this->head;
        delete this->tail;
    }
    head = new Node(*(list.head));
    //Iterating to find the new tail of the list
    if(list.tail) {
        Node *temp = head;
        for (int i = 0; i < list.length - 1; i++)
            temp = temp->next;
        this->tail = temp;
    }
    return *this;
};

// add a new element to the back of LinkedList
void LinkedList::pushTail(Member data){
    if(head){
        if(length==1) {
            tail = new Node(data);
            head->next = tail;
        }
        else{
            tail->next = new Node(data);
            tail = tail->next;
        }

    }
    else{
        head = new Node(data);

    }
    length++;
};

LinkedList::~LinkedList(){
    delete this->head;
};
