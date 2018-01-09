#include <vector>
#include <map>
#include <set>
#include <iterator>
using namespace std;

class Node{
public:
    int value;
    vector<int>* coins;
    map<set<int>,vector<int>*>* considered;
    Node(int value,int numOfCoins){
        this->value = value;
        this->coins = new vector<int>(numOfCoins);
        this->considered =new map<set<int>,vector<int>*>();
    }
    ~Node(){
        delete coins;
        map<set<int>,vector<int>*>::iterator it;
        for(it=considered->begin();it!=considered->end();it++){
            delete it->second;
        }
    }
};
