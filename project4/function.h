#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <string>
#include <math.h>
#include "variable.h"


/*
An abstract subclass of node class and represents the functions.
Due to the different functionality, we have variable and function classes.
 */
class Function : public Node{

public:

	// represents the list of variables that are inputs of this function
	vector<Variable *> inputs;

	// represents the output variable of the function.
	Variable * output;
    Function();
	Function(int _id, string _name=""); // custom constructor

	~Function(); // destructor

	// adds given variable as an input of this function
	void addInput(Variable *input);

	// sets the output variable of the function
	void setOutput(Variable *_output);

	// following two functions are inherited features from node class
	vector<Node *> getIncomings();
	vector<Node *> getOutgoings();

	// following two functions will be implemented by the subclasses of Function class

	// performs forward pass and sets the value of output variable
	virtual void doForward() = 0;

	// performs backward pass and sets the derivative values of the input variables
	virtual void doBackward() = 0;

};

/*
Subclass of function class that provides forward and backward pass functionalities 
for multiplication
 */
class Multiplication : public Function{
public:
	Multiplication(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value=this->inputs[0]->value*this->inputs[1]->value;
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative*this->inputs[1]->value;
        this->inputs[1]->derivative += this->output->derivative*this->inputs[0]->value;
	}
};

/*
Subclass of function class that provides forward and backward pass functionalities 
for addition
 */
class Addition : public Function{
public:
	Addition(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value=this->inputs[0]->value+this->inputs[1]->value;
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative;
        this->inputs[1]->derivative += this->output->derivative;
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities 
for subtraction
 */
class Subtraction : public Function{
public:
	Subtraction(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value=this->inputs[0]->value-this->inputs[1]->value;
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative;
        this->inputs[1]->derivative += -1*this->output->derivative;
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for division
 */
class Division : public Function{
public:
	Division(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value=this->inputs[0]->value/this->inputs[1]->value;
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative/this->inputs[1]->value;
        this->inputs[1]->derivative += this->output->derivative*this->inputs[0]->value*-1
                /(this->inputs[1]->value*this->inputs[1]->value);
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for sine
 */
class Sine : public Function{
public:
	Sine(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = sin(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative*cos(this->inputs[0]->value);
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for cosine
 */
class Cosine : public Function{
public:
	Cosine(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = cos(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative*-1*sin(this->inputs[0]->value);
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for tangent
 */
class Tangent : public Function{
public:
	Tangent(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = tan(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative*
                (1+tan(this->inputs[0]->value)*tan(this->inputs[0]->value));
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for ArcSine
 */
class ArcSine : public Function{
public:
	ArcSine(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = asin(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative/
                sqrt(1-this->inputs[0]->value*this->inputs[0]->value);
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for ArcCosine
 */
class ArcCosine : public Function{
public:
	ArcCosine(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = acos(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += -1*this->output->derivative/
                sqrt(1-this->inputs[0]->value*this->inputs[0]->value);
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for ArcTangent
 */
class ArcTangent : public Function{
public:
	ArcTangent(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = atan(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative/
                (1+this->inputs[0]->value*this->inputs[0]->value);
    }
};
/*
Subclass of function class that provides forward and backward pass functionalities
for exponential
 */
class Exponential : public Function{
public:
	Exponential(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = exp(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative*exp(this->inputs[0]->value);
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for logarithmic function base e
 */
class Log : public Function{
public:
	Log(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = log(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative/this->inputs[0]->value;
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for logarithmic function base 10
 */
class Log10 : public Function{
public:
	Log10(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = log10(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative/(this->inputs[0]->value*log(10));
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for Square Root
 */
class Sqrt : public Function{
public:
	Sqrt(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = sqrt(this->inputs[0]->value);
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative/(2*sqrt(this->inputs[0]->value));
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for power
 */
class Power : public Function{
public:
	Power(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value = pow(this->inputs[0]->value,this->inputs[1]->value);
	}
	void doBackward(){
		this->inputs[0]->derivative += this->output->derivative * this->inputs[1]->value
										  * pow(this->inputs[0]->value, this->inputs[1]->value - 1);
		this->inputs[1]->derivative += this->output->derivative * log(this->inputs[0]->value)
										  * pow(this->inputs[0]->value, this->inputs[1]->value);
    }
};

/*
Subclass of function class that provides forward and backward pass functionalities
for Identity
 */
class Identity : public Function{
public:
	Identity(int _id, string _name=""){
        this->id = _id;
        this->name = _name;
    }
	void doForward(){
		this->output->value += this->inputs[0]->value;
	}
	void doBackward(){
        this->inputs[0]->derivative += this->output->derivative;
    }
};

#endif // VARIABLE
