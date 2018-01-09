#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "graph.h"
#include <sstream>
#include <iterator>
#include <climits>

// helper function that checks whether the given string is number or not.
bool isNumber(const string& s) {
	char* end = 0;
	double val = strtod(s.c_str(), &end);
	return end != s.c_str() && val != HUGE_VAL;
}
Graph::Graph(){}

Graph::~Graph(){
    for(int i=0;i<idCount;i++){
        int type = this->type[i];
        if(type==0){
            Variable* n = vars[i+1];
            delete n;
        }
        else{
            Function* n = fncs[i+1];
            delete n;
        }
    }
}

int Graph::getVariable(string inp){
	int res;
	if( isNumber(inp) ){
		double val = stod(inp.c_str());
		idCount++;
		name[idCount] = inp;
		vars[idCount] = new Variable(idCount, inp, val);
		res = idCount;
		type[idCount] = VARIABLE;
	}
	else{
		if(id.find(inp)==id.end()){
			idCount++;
			id[inp] = idCount;
			name[idCount] = inp;
			vars[idCount] = new Variable(idCount, inp);
			res = idCount;
			type[idCount] = VARIABLE;
		}
		else{
			res = id[inp];
		}
	}
	// printf("%s -> %d\n",inp.c_str(), res);
	return res;
}

int Graph::getFunction(string fnc){
	idCount++;
	name[idCount] = fnc;
	type[idCount] = FUNCTION;
	Function *f;
	// cout << "new Function: " << fnc << endl;
	if(fnc.compare("mult")==0)
		f = new Multiplication(idCount, fnc);
	else if(fnc.compare("add")==0)
		f = new Addition(idCount, fnc);
	else if(fnc.compare("subs")==0)
		f = new Subtraction(idCount, fnc);
	else if(fnc.compare("divide")==0)
		f = new Division(idCount, fnc);

	else if(fnc.compare("sin")==0)
		f = new Sine(idCount, fnc);
	else if(fnc.compare("cos")==0)
		f = new Cosine(idCount, fnc);
	else if(fnc.compare("identity")==0)
		f = new Identity(idCount, fnc);
	else if(fnc.compare("tan")==0)
		f = new Tangent(idCount, fnc);
	else if(fnc.compare("acos")==0)
		f = new ArcCosine(idCount, fnc);
	else if(fnc.compare("asin")==0)
		f = new ArcSine(idCount, fnc);
	else if(fnc.compare("atan")==0)
		f = new ArcTangent(idCount, fnc);
	else if(fnc.compare("exp")==0)
		f = new Exponential(idCount, fnc);
	else if(fnc.compare("log")==0)
		f = new Log(idCount, fnc);
	else if(fnc.compare("log10")==0)
		f = new Log10(idCount, fnc);
	else if(fnc.compare("pow")==0)
		f = new Power(idCount, fnc);
	else if(fnc.compare("sqrt")==0)
		f = new Sqrt(idCount, fnc);
	fncs[idCount] = f;
	return idCount;
}

void Graph::addUnaryFunction(string fnc, string inp, string out){

	int fId = getFunction(fnc);
	int inpId = getVariable(inp);
	int outId = getVariable(out);
	fncs[fId]->addInput(vars[inpId]);
	fncs[fId]->setOutput(vars[outId]);

	vars[inpId]->addTo(fncs[fId]);
	vars[outId]->setFrom(fncs[fId]);
}

void Graph::addBinaryFunction(string fnc, string inp1, string inp2, string out){
	int fId = getFunction(fnc);
	int inp1Id = getVariable(inp1);
	int inp2Id = getVariable(inp2);
	int outId = getVariable(out);
	fncs[fId]->addInput(vars[inp1Id]);
	fncs[fId]->addInput(vars[inp2Id]);
	fncs[fId]->setOutput(vars[outId]);

	vars[inp1Id]->addTo(fncs[fId]);
	vars[inp2Id]->addTo(fncs[fId]);
	vars[outId]->setFrom(fncs[fId]);
}
void Graph::addAssignment(string lvalue, string rvalue){

}
template <class Container>
void split1(const string& str, Container& cont){
	istringstream iss(str);
	copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
			back_inserter(cont));
}
void Graph::readGraph(string fileName){
	ifstream infile(fileName);
	string line;
	while(infile.peek()!=EOF){
		getline(infile, line);
		vector<string> words;
		split1(line,words);
		if(words[0].compare("input")==0){
			inputNodes.push_back(getVariable(words[1]));
		}
		else if(words[0].compare("output")==0){
			this->outputNode = getVariable(words[1]);
            vars[outputNode]->derivative = 1;
		}
		else{
			int size=words.size();
			if(size==3){
				addUnaryFunction("identity",words[2],words[0]);
			}
			else if(size==4){
				addUnaryFunction(words[2],words[3],words[0]);
			}
			else{
				addBinaryFunction(words[2],words[3],words[4],words[0]);
			}
		}
	}
}
//helper function that returns next node to process in topological sorting process
int indegreeZero(vector<bool>& visited,vector<int>& indegrees,int idCount){
    for(int i = 0;i<idCount;i++){
        if(visited[i]==false && indegrees[i]==0)
            return i;
    }
    return -1;
}
//Applying topological sort to check for possible cycles
bool Graph::isCyclic(){
    int numOfVisited = 0;
    vector<bool> visited(idCount,false);
    vector<int> inDegrees(idCount,INT_MAX);
    for(int i=0;i<idCount;i++){
        int type =this->type[i+1];
        if(type==1){
            Function* n;
            n = fncs[i+1];
            inDegrees[i] = n->getIncomings().size();
        }
        else{
            Variable* n;
            n= vars[i+1];
            if(n->from == nullptr){
                inDegrees[i] = 0;
            }
            else
                inDegrees[i] = 1;
        }
    }
    numOfVisited=indegreeZero(visited,inDegrees,idCount);
    for(int i=0;i<idCount;i++){
        int curNode = numOfVisited;
        if(numOfVisited==-1)
            return true;
        int type =this->type[curNode+1];
        if(type == 0){
            Variable* n = vars[curNode+1];
            for(int i=0;i<n->to.size();i++){
                Function* f = n->to[i];
                inDegrees[f->id-1]-=1;
            }
            visited[curNode] = true;
        }
        else{
            Function* f =fncs[curNode+1];
            int outID = f->output->id;
            inDegrees[outID-1]-=1;
            visited[curNode] = true;
        }
        numOfVisited=indegreeZero(visited,inDegrees,idCount);
    }
	return false;
}
//Applying topological sort to find value of output node
double Graph::forwardPass(vector<double> inputValues){
	//assigning values
    for(int i=0;i < inputNodes.size();i++){
		Variable* v = vars[this->inputNodes[i]];
		v->value = inputValues[i];
	}
	int numOfVisited = 0;
	vector<bool> visited(idCount,false);
	vector<int> inDegrees(idCount,INT_MAX);
	//Indegree count
    for(int i=0;i<idCount;i++){
		int type =this->type[i+1];
		if(type==1){
            Function* n;
			n = fncs[i+1];
			inDegrees[i] = n->getIncomings().size();
		}
		else{
            Variable* n;
			n= vars[i+1];
			if(n->from == nullptr){
				inDegrees[i] = 0;
			}
			else
				inDegrees[i] = 1;
		}
	}
    //Assigning values in topological order
	while(numOfVisited!=-1){
		int curNode = numOfVisited;
		int type =this->type[curNode+1];
		if(type == 0){
			Variable* n = vars[curNode+1];
			for(int i=0;i<n->to.size();i++){
				Function* f = n->to[i];
				inDegrees[f->id-1]-=1;
			}
            visited[curNode] = true;
		}
		else{
			Function* f =fncs[curNode+1];
			f->doForward();
            int outID = f->output->id;
            inDegrees[outID-1]-=1;
            visited[curNode] = true;
        }
		numOfVisited=indegreeZero(visited,inDegrees,idCount);
	}
	Variable* n = vars[outputNode];
	return n->value;
}
//Automatic differentiation by applying topological sorting
vector<double> Graph::backwardPass(){
    int numOfVisited = 0;
    vector<bool> visited(idCount,false);
    vector<int> inDegrees(idCount,INT_MAX);
    //Indegree count and assigning derivatives to 0
    for(int i=0;i<idCount;i++){
        int type =this->type[i+1];
        if(type==0){
            Variable* n;
            n = vars[i+1];
            n->derivative = 0;
            inDegrees[i] = n->getOutgoings().size();
        }
        else{
            Function* n;
            n= fncs[i+1];
            inDegrees[i] = 1;
        }
    }
    //Assigning last node's derivative to 1
    Variable* n;
    n = vars[outputNode];
    n->derivative = 1;
    //Applying doBackward methods in topological order
    numOfVisited=indegreeZero(visited,inDegrees,idCount);
    while(numOfVisited!=-1){
        int curNode = numOfVisited;
        int type =this->type[curNode+1];
        if(type == 1){
            Function* n = fncs[curNode+1];
            n->doBackward();
            for(int i=0;i<n->inputs.size();i++){
                Variable* f = n->inputs[i];
                inDegrees[f->id-1]-=1;
            }
            visited[curNode] = true;
        }
        else{
            Variable* n = vars[curNode+1];
            if(n->from!= nullptr) {
                int outID = n->from->id;
                inDegrees[outID - 1] -= 1;
            }
            visited[curNode] = true;
        }
        numOfVisited=indegreeZero(visited,inDegrees,idCount);
    }
    vector<double> t;
    for(int i=0;i<inputNodes.size();i++){
        Variable* n = vars[inputNodes[i]];
        t.push_back(n->derivative);
    }
    return t;
}
