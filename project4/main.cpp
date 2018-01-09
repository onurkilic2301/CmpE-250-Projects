#include "graph.h"
#include <sstream>
#include <iterator>
using namespace std;
template <class Container>
void split2(const string& str, Container& cont){
    istringstream iss(str);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(cont));
}
int main(int argc,char* argv[]){
	if (argc != 5) {
		return 1;
	}
	Graph* g = new Graph();
	g->readGraph(argv[1]);
	ifstream infile(argv[2]);
    ofstream out1;
    ofstream out2;
    out1.open(argv[3]);
    out2.open(argv[4]);
    out1.precision(16);
    out2.precision(16);
	if(g->isCyclic()){
		out1<<"ERROR: COMPTUATION GRAPH HAS CYCLE!" <<endl;
        out2<<"ERROR: COMPTUATION GRAPH HAS CYCLE!" <<endl;
	}
	else{
		string line;
		vector<string> input;
		getline(infile, line);
		split2(line,input);
        string name = g->getOutputName();
        out1<<name<<endl;
        for(int i=0;i<input.size();i++){
            out2<<"d"<<name<<"/d"<<input[i]<<" ";
        }
        out2<<endl;
        vector<double> val;
		while(infile.peek()!=EOF){

			for(int i=0;i<input.size();i++){
				double x;
                infile >> x;
				val.push_back(x);
			}
            double n = g->forwardPass(val);
            vector<double> derivative = g->backwardPass();
            out1 << n<< endl;
            for(int i=0;i<input.size();i++){
                out2<< derivative[i]<<" ";
            }
            out2<<endl;
            val.clear();
            getline(infile,line);
		}

	}
    delete g;
    out1.close();
    out2.close();
	return 0;
}
