#ifndef __KOHONEN_H__
#define __KOHONEN_H__

#include <fstream>
#include <cfloat>
#include <vector>
#include <queue>

#include "neighborhood.h"
#include "../network.h"

#define INFINITE DBL_MAX

using namespace std;



template < neuronTemplate Neuron, 
			neighborhoodFunctionTemplate NeighborhoodFunction,
			distanceTemplate Distance, 
			class Type>
class Kohonen : public Network <Type>
{
public:
	Kohonen(int, int);
	Kohonen(int, int, int);
	~Kohonen();
	void initialize(int, int);
	void initializeConnections(vector< vector <int> >);
	void initialize(int, int, int);
	Type* answer(Type*){}
	int cluster(Type*);
	void train(Type**, int);
	void train(Type**, Type**,int);
	void setParameters(double, int);
	void print();
	void printConnections();
	
protected:
	Neighborhood<Neuron, NeighborhoodFunction,Distance, Type> *neighborhoods;
	void initializeGrid(int, int);
	int numNeurons;
	int numWeights;
	int numEpochs;
	int neighborhoodSize;
	double learningRate;
	void adjustWeights(int);
};

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::Kohonen(int numNeurons, int numWeights)
{
	this->numNeurons = 0;
	if (numNeurons != 0);
		initialize(numNeurons, numWeights);
}

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::Kohonen(int numLines, int numColumns, int numWeights)
{
	this->numNeurons = 0;
	if (numLines != 0 && numColumns != 0);
		initialize(numLines, numColumns, numWeights);
}

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::~Kohonen()
{
	if (this->numNeurons != 0)
		delete[] this->neighborhoods;
}

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::initialize(int numNeurons, int numWeights)
{
	this->numNeurons = numNeurons;
	this->numWeights = numWeights;
	this->neighborhoods = new Neighborhood<Neuron, NeighborhoodFunction, Distance, Type>[this->numNeurons];
	this->numEpochs = 500*numNeurons;
	for (int i = 0; i < numNeurons; i++)
	{
		this->neighborhoods[i].initialize(numWeights);
		this->neighborhoods[i].setId(i);
	}
	
}

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::initializeConnections(vector< vector <int> > connections)
{
	for (int i = 0; i < this->numNeurons; i++)
	{
		for (int j = 0; j < connections[i].size(); j++)
		{
			Neuron<Distance, Type>* connectionNeuron = this->neighborhoods[ connections[i][j] ].getCentral();
			this->neighborhoods[i].addConnection(connectionNeuron);
		}
			
	}
}

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::initialize(int numLines, int numColumns, int numWeights)
{
	this->numNeurons = numLines * numColumns;
	this->numWeights = numWeights;
	this->neighborhoods = new Neighborhood<Neuron, NeighborhoodFunction, Distance, Type>[this->numNeurons];
	this->numEpochs = 500*numNeurons;
	for (int i = 0; i < this->numNeurons; i++)
	{
		this->neighborhoods[i].initialize(numWeights);
		this->neighborhoods[i].setId(i);
	}
	
	initializeGrid(numLines, numColumns);
}

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::initializeGrid(int numLines, int numColumns)
{
	for (int i = 0; i < numLines; i++)
	{
		for (int j = 0; j < numColumns; j++)
		{
			if (i > 0)
			{
				Neuron<Distance, Type>* connectionNeuron = this->neighborhoods[ ((i-1) * numLines +  j ) ].getCentral();
				this->neighborhoods[(i * numLines + j)].addConnection(connectionNeuron);
			}
			if (i < numLines -1)
			{
				Neuron<Distance, Type>* connectionNeuron = this->neighborhoods[ (i+1) * numLines +  j ].getCentral();
				this->neighborhoods[(i * numLines  + j)].addConnection(connectionNeuron);
			}
			if (j > 0)
			{
				Neuron<Distance, Type>* connectionNeuron = this->neighborhoods[ i * numLines + (j-1) ].getCentral();
				this->neighborhoods[(i * numLines + j)].addConnection(connectionNeuron);
			}
			if (j < numColumns -1)
			{
				Neuron<Distance, Type>* connectionNeuron = this->neighborhoods[ i * numLines + (j+1) ].getCentral();
				this->neighborhoods[(i * numLines + j)].addConnection(connectionNeuron);
			}
		}
	}
}


template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::setParameters(double learningRate, int neighborhoodSize)
{
	this->learningRate = learningRate;
	this->neighborhoodSize = neighborhoodSize;
}


template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::train(Type** inputSet, int numInputs = 0)
{
	cout << this->numEpochs << endl;
	for (int i = 0; i < this->numEpochs; i++)
	{
		for (int i = 0; i < numInputs; i++)
		{
			Type minimumDistance = INFINITE;
			int minimumIndex = 0;
			for (int j = 0; j < this->numNeurons; j++)
			{
				Type currentDistance = neighborhoods[j].distanceTo(inputSet[i]);
				if (currentDistance < minimumDistance)
				{
					minimumDistance = currentDistance;
					minimumIndex = j;
				}
			}
			adjustWeights(minimumIndex);
		}
	}
}

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::train(Type** inputSet, Type** desired, int numInputs)
{
	this->train(inputSet, numInputs);
}




template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::adjustWeights(int winnerNeuron)
{
	queue<Neuron<Distance, Type>* > neurons;
	queue<int> distances;
	
	neurons.push(neighborhoods[winnerNeuron].getCentral());
	distances.push(0);
	
	Neuron<Distance, Type>* currentNeuron;
	int currentDistance;
	
	bool condition = true;
	//cout << "Adjusting weights" << endl;
	while(condition == true)
	{
		currentNeuron = neurons.front();
		currentDistance = distances.front();
		vector<Neuron<Distance, Type>* > connections = neighborhoods[currentNeuron->getId()].getConnections();
		for (int i = 0; i < connections.size(); i++)
		{
			neurons.push(connections[i]);
			distances.push(currentDistance + 1);
		}
		//cout << currentDistance << endl;
		neighborhoods[currentNeuron->getId()].adjustWeights(this->learningRate, currentDistance, this->neighborhoodSize);
		
		neurons.pop();
		distances.pop();
		
		if (neurons.empty() or currentDistance > this->neighborhoodSize)
			condition = false;
	}
	
	for (int i = 0; i < this->numNeurons; i++)
		neighborhoods[i].reinitializeAdjusted();
	
	
}

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
int Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::cluster(Type* input)
{
	Type minimumDistance = INFINITE;
	int minimumIndex = 0;
	for (int j = 0; j < this->numNeurons; j++)
	{
		Type currentDistance = neighborhoods[j].distanceTo(input);
		if (currentDistance < minimumDistance)
		{
			minimumDistance = currentDistance;
			minimumIndex = j;
		}
	}
	return neighborhoods[minimumIndex].getId();
}

template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::print()
{
	for (int i = 0; i < this->numNeurons; i++)
		neighborhoods[i].print();
}


template < neuronTemplate Neuron, neighborhoodFunctionTemplate NeighborhoodFunction, distanceTemplate Distance, class Type>
void Kohonen < Neuron, NeighborhoodFunction,  Distance, Type >::printConnections()
{
	for (int i = 0; i < this->numNeurons; i++)
		neighborhoods[i].printConnections();
}

#endif


