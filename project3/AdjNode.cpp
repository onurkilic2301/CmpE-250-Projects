#include <vector>

using namespace std;
class AdjNode{
public:
    int nodeValue;
    int dist;
    vector<int>* thiefs;
    AdjNode* next;
    AdjNode(int nodeValue,int dist){
        this->nodeValue = nodeValue;
        this->dist = dist;
        this->thiefs = new vector<int>();
        this->next= nullptr;
    }
    ~AdjNode(){
        delete thiefs;
        if(next)
            delete next;
    }
};

class AdjList{
public:
    AdjNode* head;
    AdjNode* tail;
    int lenght;
    AdjList(){
        head = nullptr;
        tail = nullptr;
        lenght = 0;
    }
    ~AdjList(){
        delete head;
    }
    void push_back(AdjNode* n){
        if(head){
            if(this->lenght==1) {
                tail = n;
                head->next = tail;
            }
            else{
                tail->next = n;
                tail = n;
            }

        }
        else{
            head = n;

        }
        this->lenght++;
    }
};



