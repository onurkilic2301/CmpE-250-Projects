#include <vector>
#include <set>
#include <climits>
using namespace std;
class Traveller{
public:
    set<int>* coins;
    vector<int>* path;
    int dist = INT_MAX;
    int startNode = -1;
    Traveller(int startNode){
        this->startNode =startNode;
        this->coins= new set<int>();
        this->path = new vector<int>();
    }
    ~Traveller(){
        delete path;
        delete coins;
    }
};



