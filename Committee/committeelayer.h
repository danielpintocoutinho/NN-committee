#ifndef __COMMITTEELAYER_H__
#define __COMMITTEELAYER_H__

#include <cfloat>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "templateDefinitions.h"
#include "activationfunction.h"
#include "network.h"

#define INFINITE DBL_MAX


template <class Type>
class CommitteeLayer
{
public:
	CommitteeLayer();
	~CommitteeLayer();
	void train(Type**, Type**, int);
	Type* answer(Type*);
	void add(Network*);
	void print();
private:
	int getIndex(Network*);
	vector< Network* > networks;
	int numNetworks;
	double learningRate;
	double minimumError;
	int maximumEpochs;
	
};

template <class Type>	
CommitteeLayer <Type>::CommitteeLayer()
{
	this->numNetworks = 0;
}

template <class Type>	
CommitteeLayer <Type>::~CommitteeLayer()
{
	if (numNetworks != 0)
		this->networks.clear();
}

template <class Type>
void CommitteeLayer<Type>::add(Network* newNetwork)
{
	networks.push_back(newNetwork);
	numNetworks += 1;
}

template <class Type>
int CommitteeLayer<Type>::getIndex(Network* network)
{
	for (int i = 0; i < networks.size(); i++)
		if (networks[i] == network)
			return i;
	return -1;
}

template <class Type>	
Type* CommitteeLayer <Type>::answer(Type* input)
{/*
	Type* answer = new Type[this->numNeurons];
	for (int i = 0; i < this->numNeurons; i++)
		answer[i] = this->neurons[i].activationFunction(input);
	return answer;*/
}

template <class Type>	
void CommitteeLayer <Type>::train(Type** inputSet, Type** intended , int numInputs)
{/*
	bool condition = true;
	double previousError = 0;
	double currentError;
	int epochs = 0;
	
	Type** obtained;
	obtained = new Type*[numInputs];
	for (int i = 0; i < numInputs; i++)
		obtained[i] = new Type[this->numNeurons];
	
	for (int i = 0; i < this->numNeurons; i++)
		neurons[i].print();
	
	
	while(condition)
	{		
		
		for (int i = 0; i < numInputs; i++)
		{
			for (int j = 0; j < this->numNeurons; j++)
			{
				obtained[i][j] = this->neurons[j].activationFunction(inputSet[i]);
				this->neurons[j].adjustWeights(inputSet[i], intended[i][j], this->learningRate);
			}
		}
		
		/** Calculando o erro medio */
/*
		double mediumError = 0;
		for (int i = 0; i < numInputs; i++)
		{
			double localError = 0;
			for (int j = 0; j < this->numNeurons; j++)
			{
				localError += pow((intended[i][j] - obtained[i][j]),2);
			}
			localError /= 2;
			mediumError += localError;
		}
		mediumError /= numInputs;
		
		/** pesos e condicao de parada */
/*		currentError = mediumError; //Calcular erro medio;
		if ( ((abs(currentError - previousError) < this->minimumError)) ||  (epochs > this->maximumEpochs))
			condition = false;
		epochs += 1;
		previousError = currentError; // Calcular erro medio;

	}
	
	for (int i = 0; i < numInputs; i++)
		delete[] obtained[i];
	delete[] obtained;*/
	
}


template <class Type>	
void CommitteeLayer <Type>::print()
{
	for (int i = 0; i < this->numNetworks; i++)
	{
		cout << "Network " << i << ":" << endl;
		this->networks[i]->print();
		cout << endl;
	}
}


#endif