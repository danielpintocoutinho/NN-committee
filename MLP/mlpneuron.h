#ifndef __MLPNEURON_H__
#define __MLPNEURON_H__


#include <cstdlib>
#include <iostream>

#include "../Common/templateDefinitions.h"

using namespace std;

#define FACTOR 10000

template < activationTemplate Activation, class Type>
class MLPNeuron
{
public:
	MLPNeuron(int);
	~MLPNeuron();
	void initialize(int);
	Type activationFunction(Type*);
	void adjustWeights(Type* , Type , double , int);
	void adjustWeights(Type*, double, const Type*, const Type* , int );
	void setThreshold(Type);
	Type getDelta();
	const Type* getWeights();
	void print();
	
private:
	Type* weights;
	Type threshold;
	Type delta;
	int numWeights;
	Activation<Type> activation;
	void initializeWeights();
};

template < activationTemplate Activation, class Type>
MLPNeuron<Activation, Type>::MLPNeuron(int numWeights = 0)
{
	this->numWeights = 0;
	if (numWeights != 0)
		initialize(numWeights);
}

template <activationTemplate Activation, class Type>
MLPNeuron<Activation, Type>::~MLPNeuron()
{
	if (this->numWeights != 0)
		delete[] this->weights;
}

template < activationTemplate Activation, class Type>
void MLPNeuron<Activation, Type>::initialize(int numWeights)
{
	this->numWeights = numWeights;
	weights = new Type[numWeights];
	initializeWeights();
}

template <activationTemplate Activation, class Type>
void  MLPNeuron<Activation, Type>::initializeWeights()
{
	for (int i = 0; i < this->numWeights; i++)
		this->weights[i] = (rand()%FACTOR)/(double) FACTOR;
}

template <activationTemplate Activation, class Type>
void MLPNeuron<Activation, Type>::setThreshold(Type newThreshold)
{
	this->threshold = newThreshold;
}

template <activationTemplate Activation, class Type>
Type MLPNeuron<Activation, Type>::activationFunction(Type* input)
{
	Type result = 0;
	for (int i = 0; i < this->numWeights; i++)
		result += this->weights[i] * input[i];
	
	this->activation.exec(result, this->threshold);
	return this->activation.getResult(); 
}


template <activationTemplate Activation, class Type>
void MLPNeuron<Activation, Type>::adjustWeights(Type* input, Type desired, double learningRate, int index)
{
	Type result = 0;
	for (int i = 0; i < this->numWeights; i++)
		result += this->weights[i] * input[i];

	this->activation.derived(result, this->threshold);
	this->delta = (desired - this->activation.getResult() ) * this->activation.getDerivedResult();

/*	if (desired == 1 )
	{
		cout << endl << "Neuron OF LAYER: " << index << endl;

		Type value = 0;
		for (int i = 0; i < this->numWeights; i++)
			value += this->weights[i] * input[i];

		Type result = 1;
		Type denominator = 1 + exp(-value);
		result /= denominator;
		//cout << "den: " << denominator << " | res: " << result << endl;
		
		//this->activation.exec(value, this->threshold);
		cout << endl;
		cout << "desired: " << desired << " | act func: " <<  this->activation.getResult()  << endl;
		cout << "Neurons: "<< endl;
		print();
		cout << "Input: " << endl;
		for (int i = 0; i < this->numWeights; i++)
			cout << input[i] << " ";
		cout << endl;

	}*/
	

	Type factor = learningRate * this->delta;
	

	for (int i = 0; i < this->numWeights; i++)
	{
		this->weights[i] += factor * input[i];
	}
	/*if (desired == 1)
	{
		Type value = 0;
		for (int i = 0; i < this->numWeights; i++)
			value += this->weights[i] * input[i];

		Type result = 1;
		Type denominator = 1 + exp(-value);
		result /= denominator;
		//cout << "den: " << denominator << " | res: " << result << endl;

		this->activation.exec(value, this->threshold);
		cout << endl << "desired: " << desired << " | act func: " <<  this->activation.getResult()  << endl;
		cout << "Neurons:" << endl;	
		print();
		cout << "Input: " << endl;
		for (int i = 0; i < this->numWeights; i++)
			cout << input[i] << " ";

		cout << endl << flush;
	}*/
}
template <activationTemplate Activation, class Type>
void MLPNeuron<Activation, Type>::adjustWeights(Type* input, double learningRate, const Type* nextLayerWeights, const Type* nextLayerDeltas, int numNeuronsNextLayer)
{
	Type result = 0;
	for (int i = 0; i < this->numWeights; i++)
		result += this->weights[i] * input[i];

	this->activation.derived(result, this->threshold);


	this->delta = 0;
	for (int i = 0; i < numNeuronsNextLayer; i++)
		this->delta += nextLayerWeights[i] * nextLayerDeltas[i];
	this->delta *= this->activation.getDerivedResult();


	Type factor = learningRate * this->delta;

	for (int i = 0; i < this->numWeights; i++)
	{
		this->weights[i] += factor * input[i];
	}
}


template <activationTemplate Activation, class Type>
Type  MLPNeuron<Activation, Type>::getDelta()
{
	return this->delta;
}

template <activationTemplate Activation, class Type>
const Type* MLPNeuron<Activation, Type>::getWeights()
{
	return weights;
}


template <activationTemplate Activation, class Type>
void MLPNeuron<Activation, Type>::print()
{
	for (int i = 0; i < this->numWeights; i++)
		cout << this->weights[i] <<  " ";
	cout << endl;
}


#endif
