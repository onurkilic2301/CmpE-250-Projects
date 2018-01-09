
#include "function.h"

Function::Function(){

}
Function::~Function() {

}
Function::Function(int _id, string _name){
	this->id = _id;
	this->name = _name;

}

void Function::addInput(Variable *input){
	this->inputs.push_back(input);
}

void Function::setOutput(Variable *_output){
	this->output = _output;
}

vector<Node *> Function::getIncomings(){
	return (vector<Node*>&)(this->inputs);
}
vector<Node *> Function::getOutgoings(){
	return (vector<Node*>&)this->output;
}
