#include "variable.h"

Variable::Variable(){
}
Variable::Variable(int _id,string _name, double _value){
	this->id = _id;
	this->name = _name;
	this->value = _value;
    this->from = nullptr;
	this->derivative = 0;
}
Variable::~Variable(){
}

void Variable::setFrom(Function *_from){
	this->from = _from;
}

void Variable::addTo(Function *_to){
	this->to.push_back(_to);
}
vector<Node *> Variable::getIncomings(){
    vector<Node *> n;
    if(this->from) {
        n.push_back((Node *) this->from);
    }
	return n;
};
vector<Node *> Variable::getOutgoings(){
	return (vector<Node*>&)this->to;
};
