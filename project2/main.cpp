
#include <vector>
#include <iostream>
#include <queue>
#include <fstream>
using namespace std;

//order is used to store and process order info
struct order{
    double timeOfArrival;
    double timeOfOrder;
    double timeOfBrew;
    double price;
    double timeToProcess = 0;
    double timeOfExit = -1;
};

//comparePrice is to make baristaQueue to prioritize the most expensive order
struct comparePrice{
    bool operator()(const order &a,const order &b){
        return a.price < b.price;
    }
};

//compareArrival is to make exitQueue to prioritize the order with earliest arrival time
struct compareArrival{
    bool operator()(const order &a,const order &b){
        return a.timeOfArrival > b.timeOfArrival;
    }
};

int main(int argc, char* argv[]){
    // below reads the input file
    if (argc != 3) {
        cout << "Run the code with the following command: ./project1 [input_file] [output_file]" << endl;
        return 1;
    }

    cout << "input file: " << argv[1] << endl;
    cout << "output file: " << argv[2] << endl;

    int numOfCashiers,numOfOrders;
    ifstream infile(argv[1]);
    infile >> numOfCashiers;
    cout << "number of Cashiers: " << numOfCashiers << endl;
    infile >> numOfOrders;
    cout << "number of Orders: " << numOfOrders << endl;
    queue<order> inputQueue;
    //Creating orders and initializing inputQueue
    for (int i=0; i < numOfOrders; i++) {
        double arrivalTime,orderTime,brewTime,price;
        infile >> arrivalTime >> orderTime >> brewTime >> price;
        order o;
        o.timeOfArrival = arrivalTime;
        o.timeOfOrder = orderTime;
        o.timeOfBrew = brewTime;
        o.price = price;
        inputQueue.push(o);
    }

    cout << "input file has been read" << endl;

    //First Model

    //timeToCheck is created to store and get the earliest time info to process an order
    priority_queue<double,vector<double>, greater<double>> timeToCheck;
    queue<order> cashierQueue;
    order cashiers[numOfCashiers];
    //cashierInfo is used to store busy time of cashiers
    double cashierInfo[numOfCashiers];
    //baristaQueue is created to process and get the most expensive order
    priority_queue<order,vector<order>,comparePrice> baristaQueue;
    order baristas[numOfCashiers/3];
    //baristaInfo is used to store busy time of baristas
    double baristaInfo[numOfCashiers/3];
    //initializing double arrays to prevent inconvenient values
    for(int i=0;i<numOfCashiers;i++){
        cashierInfo[i]=0;
    }
    for(int i=0;i<numOfCashiers/3;i++){
        baristaInfo[i]=0;
    }
    //exitQueue is created to process order info after orders are completed
    //exitQueue prioritizes the order with earliest arrival time
    priority_queue<order,vector<order>,compareArrival> exitQueue;
    double currentTime = 0;
    int maxCashierQueue = 0;
    int maxBaristaQueue = 0;
    int processedOrders = 0;
    //Processing first order
    cashierQueue.push(inputQueue.front());
    inputQueue.pop();

    int counter=0;
    while(counter!=numOfOrders*10 && processedOrders != numOfOrders){
        bool eventDone = false;
        //Checking for new orders
        if(!timeToCheck.empty() && !inputQueue.empty() && timeToCheck.top() > inputQueue.front().timeOfArrival){
            currentTime = inputQueue.front().timeOfArrival;
            cashierQueue.push(inputQueue.front());
            inputQueue.pop();
            eventDone = true;
            //Checking cashierQueue length
            if(cashierQueue.size() > maxCashierQueue){
                maxCashierQueue = cashierQueue.size();
            }
        }
        //Checking cashiers for the event to be done
        if(!eventDone && !timeToCheck.empty()){
            currentTime = timeToCheck.top();
            timeToCheck.pop();
            for(int i=0; i<numOfCashiers && !eventDone ; i++){
                if(cashiers[i].timeToProcess == currentTime){
                    cashierInfo[i]+=cashiers[i].timeOfOrder;
                    baristaQueue.push(cashiers[i]);
                    eventDone = true;
                }
            }
            //Checking baristaQueue length
            if(baristaQueue.size() > maxBaristaQueue){
                maxBaristaQueue = baristaQueue.size();
            }
            //Checking baristas for the event to be done
            for(int i=0; i<numOfCashiers/3 && !eventDone ; i++){
                if(baristas[i].timeToProcess == currentTime){
                    baristas[i].timeOfExit = currentTime;
                    baristaInfo[i]+=baristas[i].timeOfBrew;
                    exitQueue.push(baristas[i]);
                    processedOrders++;
                    eventDone = true;
                }
            }
        }
        //Checking for empty places if cashierQueue not empty
        for(int i=0; i<numOfCashiers && !cashierQueue.empty(); i++){
            if(cashiers[i].timeToProcess <= currentTime){
                //Updating timeToCheck
                timeToCheck.push(currentTime + cashierQueue.front().timeOfOrder);
                cashierQueue.front().timeToProcess = currentTime + cashierQueue.front().timeOfOrder;
                cashiers[i] = cashierQueue.front();
                cashierQueue.pop();
            }
        }

        //Checking for empty places if baristaQueue not empty
        for(int i=0; i<numOfCashiers/3 && !baristaQueue.empty(); i++){
            if(baristas[i].timeToProcess <= currentTime){
                //Updating timeToCheck
                timeToCheck.push(currentTime + baristaQueue.top().timeOfBrew);
                baristas[i] = baristaQueue.top();
                baristas[i].timeToProcess = currentTime + baristaQueue.top().timeOfBrew;
                baristaQueue.pop();
            }
        }
        //Adding an element in cashierQueue in case timeToCheck is empty
        if(timeToCheck.empty() && !inputQueue.empty()){
            currentTime = inputQueue.front().timeOfArrival;
            cashierQueue.push(inputQueue.front());
            inputQueue.pop();

        }
        counter++;
    }

    //Printing values
    ofstream myfile;
    myfile.open (argv[2]);
    myfile.precision(2);
    myfile << fixed;
    double totalRunningTime = currentTime;
    myfile << totalRunningTime << endl;
    myfile << maxCashierQueue << endl;
    myfile << maxBaristaQueue << endl;
    for (int j = 0; j < numOfCashiers; ++j) {
        myfile << cashierInfo[j]/totalRunningTime << endl;
    }
    for (int j = 0; j < numOfCashiers/3; ++j) {
        myfile << baristaInfo[j]/totalRunningTime << endl;
    }
    while(!exitQueue.empty()){
        myfile << exitQueue.top().timeOfExit-exitQueue.top().timeOfArrival << endl;
        inputQueue.push(exitQueue.top());
        exitQueue.pop();
    }
    myfile << endl;

    //Second model

    //baristaQueues is created to process and get the most expensive orders in each barista queue
    priority_queue<order,vector<order>,comparePrice> baristaQueues[numOfCashiers/3];
    //maxBaristaQueues is created to store max length of barista queues
    int maxBaristaQueues[numOfCashiers/3];
    maxCashierQueue = 0;
    counter = 0;
    currentTime = 0;
    processedOrders = 0;
    //initializing values to 0 to process orders
    for(int i=0;i<numOfCashiers;i++){
        cashierInfo[i] = 0;
        cashiers[i].timeToProcess = 0;
    }
    for(int i=0;i<numOfCashiers/3;i++){
        maxBaristaQueues[i] = 0;
        baristaInfo[i] = 0;
        baristas[i].timeToProcess = 0;
    }
    //Processing first order
    cashierQueue.push(inputQueue.front());
    inputQueue.pop();

    while(counter < numOfOrders*10 && processedOrders!=numOfOrders){
        bool eventDone = false;
        //Checking for new orders
        if(!timeToCheck.empty() && !inputQueue.empty() && timeToCheck.top() > inputQueue.front().timeOfArrival){
            currentTime = inputQueue.front().timeOfArrival;
            cashierQueue.push(inputQueue.front());
            inputQueue.pop();
            eventDone = true;
            //Checking cashierQueue length
            if(cashierQueue.size() > maxCashierQueue){
                maxCashierQueue = cashierQueue.size();
            }
        }
        //Checking cashiers for the event to be done
        if(!eventDone && !timeToCheck.empty()){
            currentTime = timeToCheck.top();
            timeToCheck.pop();
            for(int i=0; i<numOfCashiers && !eventDone ; i++){
                if(cashiers[i].timeToProcess == currentTime){
                    cashierInfo[i]+=cashiers[i].timeOfOrder;
                    baristaQueues[i/3].push(cashiers[i]);
                    eventDone = true;
                }
            }
            //Checking baristaQueue length
            for(int i=0; i < numOfCashiers/3; i++ ){
                if(baristaQueues[i].size() > maxBaristaQueues[i]){
                    maxBaristaQueues[i] = baristaQueues[i].size();
                }
            }
            //Checking baristas for the event to be done
            for(int i=0; i<numOfCashiers/3 && !eventDone ; i++){
                if(baristas[i].timeToProcess == currentTime){
                    baristas[i].timeOfExit = currentTime;
                    baristaInfo[i]+=baristas[i].timeOfBrew;
                    exitQueue.push(baristas[i]);
                    processedOrders++;
                    eventDone = true;
                }
            }
        }
        //Checking for empty places if cashierQueue not empty
        for(int i=0; i<numOfCashiers && !cashierQueue.empty(); i++){
            if(cashiers[i].timeToProcess <= currentTime){
                //Updating timeToCheck
                timeToCheck.push(currentTime + cashierQueue.front().timeOfOrder);
                cashierQueue.front().timeToProcess = currentTime + cashierQueue.front().timeOfOrder;
                cashiers[i] = cashierQueue.front();
                cashierQueue.pop();
            }
        }
        //Checking for empty places if baristaQueues are not empty
        for(int i=0; i<numOfCashiers/3; i++){
            if(baristas[i].timeToProcess <= currentTime && !baristaQueues[i].empty()){
                //Updating timeToCheck
                timeToCheck.push(currentTime + baristaQueues[i].top().timeOfBrew);
                baristas[i] = baristaQueues[i].top();
                baristas[i].timeToProcess = currentTime + baristaQueues[i].top().timeOfBrew;
                baristaQueues[i].pop();
            }
        }
        //Adding an element in cashierQueue in case timeToCheck is empty
        if(timeToCheck.empty() && !inputQueue.empty()){
            currentTime = inputQueue.front().timeOfArrival;
            cashierQueue.push(inputQueue.front());
            inputQueue.pop();
        }
        counter++;
    }
    //Printing values;
    totalRunningTime = currentTime;
    myfile << totalRunningTime << endl;
    myfile << maxCashierQueue << endl;
    for(int i = 0; i < numOfCashiers/3; i++){
        myfile << maxBaristaQueues[i] << endl;
    }
    for (int j = 0; j < numOfCashiers; ++j) {
        myfile << cashierInfo[j]/totalRunningTime << endl;
    }
    for (int j = 0; j < numOfCashiers/3; ++j) {
        myfile << baristaInfo[j]/totalRunningTime << endl;
    }
    while(!exitQueue.empty()){
        myfile << exitQueue.top().timeOfExit-exitQueue.top().timeOfArrival << endl;
        exitQueue.pop();
    }
    myfile.close();
}








