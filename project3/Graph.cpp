#include "Node.cpp"
#include "AdjNode.cpp"
#include "Traveller.cpp"
#include <stack>
#include <queue>
using namespace std;
struct compareTravellers{
    bool operator()(Traveller* t1,Traveller* t2){
        return t1->dist > t2->dist;
    }
};
class Graph{
public:
    int numOfNodes;
    vector<Node*>* nodes;
    vector<AdjList*>* adjList;
    Graph(int numOfNodes,int numOfThiefs){
        this->numOfNodes=numOfNodes;
        this->nodes= new vector<Node*>(numOfNodes);
        this->adjList=new() vector<AdjList*>(numOfNodes);

        for(int i = 0;i<numOfNodes;i++){
            Node* n= new Node(i+1,numOfThiefs);
            (*nodes)[i] = n;
            AdjList* l = new AdjList();
            (*adjList)[i] = l;
        }
    }
    ~Graph(){
        for(int i = 0;i<numOfNodes;i++){
            Node* n = (*nodes)[i];
            delete n;
            AdjList* l = (*adjList)[i];
            delete l;
        }
        delete nodes;
        delete adjList;
    }
    //takes a Traveller t and vector of thief info on a road
    //returns if traveller can pass thieves
    bool canPassThiefs(const Traveller* t,const vector<int>* thiefs){
        set<int>* coins = t->coins;
        for(int i:*thiefs){
            if((*coins).count(i)==0)
                return false;
        }
        return true;
    }
    //returns if node has different coins than traveller has
    bool holdsDifferentCoin(const Traveller* t,int node){
        Node* n = (*this->nodes)[node-1];
        for(int i: (*n->coins)){
            if(t->coins->count(i) == 0)
                return true;
        }
        return false;
    }
    bool isAlreadyApplied(Node* tmp,Traveller* nt){
        vector<int> *dist;
        if((*tmp->considered).find(*nt->coins)!=(*tmp->considered).end()) {
            vector<int> *dist = (*tmp->considered).at(*nt->coins);
            for(int i:*dist){
                if(nt->dist>i){
                    return true;
                }
            }
            dist->push_back(nt->dist);
            return false;
        }
        else{
            dist=new vector<int>;
            dist->push_back(nt->dist);
            (*tmp->considered).insert(make_pair(*nt->coins,dist));
            return false;
        }
    }
    bool Dijkstra(int node,Traveller* t,stack<Traveller*>& checkRoute,priority_queue<Traveller*,vector<Traveller*>,compareTravellers>& reaches){
        vector<int> dist(numOfNodes,INT_MAX);
        vector<int> prev(numOfNodes,-1);
        set<int> visited;
        //Initializing source dist to 0
        dist[node-1]=0;
        int currentNode = node;
        //Applying dijkstra for current situation
        while(visited.size()!=numOfNodes){
            AdjList* list = (*adjList)[currentNode-1];
            AdjNode* an = list->head;
            visited.insert(currentNode);
            //Visiting all adjacent nodes
            while(an != nullptr){
                //Checking if node is visited
                if(visited.count(an->nodeValue)==0){
                    //Checking if can pass thiefs on the road
                    if(canPassThiefs(t,an->thiefs)){
                        //Checking if a shorter distance is possible
                        if(dist[an->nodeValue-1]>dist[currentNode-1]+an->dist){
                            dist[an->nodeValue-1] = dist[currentNode-1]+an->dist;
                            prev[an->nodeValue-1] = currentNode;
                        }
                    }
                }
                an = an->next;
            }
            //Getting the min index which is not visited
            int index,min;
            index = -1;
            min = INT_MAX;
            for(int i = 0;i<dist.size();i++){
                if(min > dist[i]&&visited.count(i+1)==0){
                    min = dist[i];
                    index = i;
                }
            }
            currentNode = index+1;
            //If no other node is reachable then finish dijkstra
            if(currentNode == 0)
                break;
        }
        //Checking if nodes with distinct coins are encountered
        for(int i=0;i<numOfNodes-1;i++){
            //Checking if node reached
            if(dist[i]<INT_MAX) {
                //Checking if shorter distance can occur
                if(reaches.empty()||!reaches.empty()&&t->dist+dist[i]<reaches.top()->dist) {
                    //Checking if node holds distinct coins
                    if(holdsDifferentCoin(t,i+1)){
                        Node *tmp = (*nodes)[i];
                        //new Traveller is created to apply dijkstra in new situation
                        Traveller *nt = new Traveller(i + 1);
                        for (int i:*t->path)
                            (*nt->path).push_back(i);
                        nt->dist = t->dist + dist[i];
                        for (int i:*t->coins)
                            (*nt->coins).insert(i);
                        for (int i:*tmp->coins)
                            (*nt->coins).insert(i);
                        if(!isAlreadyApplied(tmp,nt)) {
                            stack<int> temp;
                            int root = nt->startNode;
                            while (prev[root - 1] != -1) {
                                temp.push(prev[root - 1]);
                                root = prev[root - 1];
                            }
                            while (!temp.empty()) {
                                nt->path->push_back(temp.top());
                                temp.pop();
                            }
                            //Traveller pushed to checkRoute for further route check
                            checkRoute.push(nt);
                        }
                        else
                            delete nt;
                    }
                }
            }
        }
        //Checking if last node is reached
        if(dist[numOfNodes-1]<INT_MAX){
            //Path to last road is assigned
            t->dist = t->dist+dist[numOfNodes-1];
            stack<int> tmp;
            int root = numOfNodes;
            while(root!=-1){
                tmp.push(root);
                root = prev[root-1];
            }
            while(!tmp.empty()){
                t->path->push_back(tmp.top());
                tmp.pop();
            }
            return true;
        }
        return false;
    }

};



