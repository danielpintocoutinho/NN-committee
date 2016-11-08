#ifndef __RBFHIDDENNEURON_H__
#define __RBFHIDDENNEURON_H__

#include "../templateDefinitions.h"
#include "activationfunction.h"
#include "euclidiandistance.h"
#include <cstdlib>


#define FACTOR 10000


template <activationTemplate Activation, class Type>
class RBFHiddenNeuron
{

public:
	RBFHiddenNeuron(int);
	~RBFHiddenNeuron();
	void initialize(int);
	Type activationFunction(Type*);
	Type euclidianDistance(Type*);
	void adjustWeights(Type**, Type);
	void setSigma(Type);
	int getNumWeights();
	void print();
	void setWeights(Type*);
protected:
	Type* weights;
	Type sigma;
	int numWeights;
	Activation<Type> activation;
	EuclidianDistance<Type> distance;
	void initializeWeights();
};

template <activationTemplate Activation, class Type>
RBFHiddenNeuron<Activation, Type>::RBFHiddenNeuron(int weights = 0)
{
	this->numWeights = 0;
	if (weights != 0)
		initialize(weights);
}

template <activationTemplate Activation, class Type>
RBFHiddenNeuron<Activation, Type>::~RBFHiddenNeuron()
{
	if (this->numWeights != 0)
		delete[] this->weights;
}

template <activationTemplate Activation, class Type>
void RBFHiddenNeuron<Activation, Type>::initialize(int weights)
{
	this->numWeights = weights;
	this->weights = new Type[weights];
	initializeWeights();
}


template <activationTemplate Activation, class Type>
Type RBFHiddenNeuron<Activation, Type>::activationFunction(Type* input)
{
	Type distance = 0;
	for (int i = 0; i < this->numWeights; i++)
		distance += pow((this->weights[i] - input[i]),2);

	activation.exec(distance, this->sigma);
	return activation.getResult(); 
}

template <activationTemplate Activation, class Type>
Type RBFHiddenNeuron<Activation, Type>::euclidianDistance(Type* input)
{
	distance.exec(this->weights, input, this->numWeights);
	return distance.getResult(); 
}

template <activationTemplate Activation, class Type>
void RBFHiddenNeuron<Activation, Type>::adjustWeights(Type** inputSet, Type numInputs)
{
	Type auxWeights[this->numWeights];
	for (int i = 0; i < this->numWeights; i++)
		auxWeights[i] = 0;
	for (int i = 0; i < numInputs; i++)
		for (int j = 0; j < this->numWeights; j++)
			auxWeights[j] += inputSet[i][j];
	for (int i = 0; i < this->numWeights; i++)
		this->weights[i] = auxWeights[i]/numInputs;
	
}

template <activationTemplate Activation, class Type>
void RBFHiddenNeuron<Activation, Type>::setSigma(Type value)
{
	this->sigma = value;
}

template <activationTemplate Activation, class Type>
int RBFHiddenNeuron<Activation, Type>::getNumWeights()
{
	return this->numWeights;
}

template <activationTemplate Activation, class Type>
void RBFHiddenNeuron<Activation, Type>::print()
{
	for (int i = 0; i < this->numWeights; i++)
		cout << this->weights[i] << " ";
	cout << endl;
}

template <activationTemplate Activation, class Type>
void RBFHiddenNeuron<Activation, Type>::setWeights(Type* weights)
{
	for (int i = 0; i < this->numWeights; i++)
		this->weights[i] = weights[i];
}

template <activationTemplate Activation, class Type>
void RBFHiddenNeuron<Activation, Type>::initializeWeights()
{
	for (int i = 0; i < this->numWeights; i++)
		this->weights[i] = (rand()%FACTOR)/(double) FACTOR;
}

#endif