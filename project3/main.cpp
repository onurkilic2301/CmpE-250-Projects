
#include <iostream>
#include <fstream>
#include "Graph.cpp"
using namespace std;
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }

    //cout << "input file: " << argv[1] << endl;
    //cout << "output file: " << argv[2] << endl;
    //Start taking input
    ifstream infile(argv[1]);
    int numOfTowns,numOfRoads,numOfThiefGroups,numOfCoinholderTowns;
    infile >> numOfTowns >> numOfRoads >> numOfThiefGroups >> numOfCoinholderTowns;
    //Creating graph
    Graph* map = new Graph(numOfTowns,numOfThiefGroups);
    //Initializing coins to towns
    for(int i = 0;i<numOfCoinholderTowns;i++){
        int currentTown,numOfCoins;
        infile >> currentTown >> numOfCoins;
        //Inserting coins to towns
        for(int j = 0;j<numOfCoins;j++){
            int coin;
            infile >> coin;
            (*(*map->nodes)[currentTown-1]->coins).push_back(coin);
        }
    }
    //Initializing roads
    for(int i = 0;i<numOfRoads;i++){
        int node1,node2,distance,numOfThiefs;
        infile >> node1 >> node2 >> distance >> numOfThiefs;
        //Initializing road info both ways
        AdjNode* a1 = new AdjNode(node2,distance);
        AdjNode* a2 = new AdjNode(node1,distance);
        //Inserting thiefs to roads
        for(int j = 0;j<numOfThiefs;j++){
            int thief;
            infile >> thief;
            (*a1->thiefs).push_back(thief);
            (*a2->thiefs).push_back(thief);
        }
        //Adding Adjacent nodes to adjacency list
        (*(*map->adjList)[node1-1]).push_back(a1);
        (*(*map->adjList)[node2-1]).push_back(a2);
    }
    //Completed reading input
    //roadToLast stores each Travellers info which can reach last node
    //gives the min distance node
    priority_queue<Traveller*,vector<Traveller*>,compareTravellers> roadToLast;
    Traveller* t = new Traveller(1);
    //chekcRoute is used to check paths in different situations
    stack<Traveller*> checkRoute;
    checkRoute.push(t);
    //Finding shortest paths in different situations until all are considered
    while(!checkRoute.empty()){
        t = checkRoute.top();
        checkRoute.pop();
        bool reachesEnd=map->Dijkstra(t->startNode,t,checkRoute,roadToLast);
        if(reachesEnd)
            roadToLast.push(t);
        else
            delete t;
    }
    ofstream myfile;
    myfile.open(argv[2]);
    if(!roadToLast.empty()){
        t = roadToLast.top();
        roadToLast.pop();
        for(int i:*t->path)
            myfile << i << " ";
        myfile<<endl;
        delete t;
        while(!roadToLast.empty()){
            t = roadToLast.top();
            roadToLast.pop();
            delete t;
        }
    } else{
        myfile<<-1 <<endl;
    }
    myfile.close();
    delete map;
    return 0;
}


