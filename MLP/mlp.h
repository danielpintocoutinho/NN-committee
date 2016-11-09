#ifndef __MLP_H__
#define __MLP_H__

#include <iostream>
#include <cmath>

#include "../Common/network.h"
#include "../Common/templateDefinitions.h"


using namespace std;

template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
class MLP : public Network<Type>
{
public:
	MLP(int, int, int*);
	MLP(int, int, initializer_list<int>);
	~MLP();
	void initialize(int, int, int*);
	void initialize(int, int, initializer_list<int>);
	void train(Type**, Type**, int);
	void setParameters(double, double, int, Type);
	Type* answer(Type*);
	void print();
private:
	Layer<Neuron, Activation, Type>* layers;
	int numLayers;
	int numAttributes;
	double minimumError;
	int maximumEpochs;

};


template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
MLP<Layer, Neuron, Activation, Type>::MLP(int numLayers = 0, int numAttributes = 0, int* numNeuronsLayers = 0)
{
	this->numLayers = 0;
	if (numLayers != 0)
		initialize(numLayers, numAttributes, numNeuronsLayers);
}

template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
MLP<Layer, Neuron, Activation, Type>::~MLP()
{
	if (this->numLayers != 0)
		delete[] layers;
}

template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
MLP<Layer, Neuron, Activation, Type>::MLP(int numLayers , int numAttributes , initializer_list<int> numNeuronsLayers)
{
	if (numLayers != 0)
		initialize(numLayers, numAttributes, numNeuronsLayers);
}


template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLP<Layer, Neuron, Activation, Type>::initialize(int numLayers, int numAttributes, int* numNeuronsLayers)
{
	this->numLayers = numLayers;
	this->numAttributes = numAttributes;
	this->layers = new Layer<Neuron, Activation, Type>[numLayers];
	this->layers[0].initialize(numNeuronsLayers[0], numAttributes);
	for (int i = 1; i < numLayers; i++)
		this->layers[i].initialize(numNeuronsLayers[i], numNeuronsLayers[i-1]);
	this->layers[this->numLayers - 1].setOutputLayer();
}

template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLP<Layer, Neuron, Activation, Type>::initialize(int numLayers, int numAttributes, initializer_list<int> numNeuronsLayers)
{
	this->numLayers = numLayers;
	this->numAttributes = numAttributes;
	this->layers = new Layer<Neuron, Activation, Type>[numLayers];
	initializer_list<int>::iterator it = numNeuronsLayers.begin();
	layers[0].initialize(*it, numAttributes);
	it++;
	for (int i = 1; i < numLayers; i++, it++)
	{
		layers[i].initialize(*it, *(it-1));
	}
	this->layers[this->numLayers - 1].setOutputLayer();
}


template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
Type* MLP<Layer, Neuron, Activation, Type>::answer(Type* input)
{
	int numClasses = layers[numLayers - 1].getNumNeurons();
	Type* output = new Type[numClasses];

	this->layers[0].answer(input);
	for (int i = 1; i < this->numLayers; i++)
	{
		Type* currentOutput = this->layers[i].answer(this->layers[i-1].getOutput());
		if (i == numLayers - 1)
		{
			for (int i = 0; i < numClasses; i++)
				output[i] = currentOutput[i];

		}
	}
	return output;
}

template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLP<Layer, Neuron, Activation, Type>::setParameters(double newLearningRate, double newMinimumError, int newMaximumEpochs, Type threshold)
{
	this->maximumEpochs = newMaximumEpochs;
	this->minimumError = newMinimumError;
	for (int i = 0; i < numLayers; i++)
		this->layers[i].setParameters( newLearningRate,  newMinimumError,  newMaximumEpochs,  threshold);
}


template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLP<Layer, Neuron, Activation, Type>::train(Type** input, Type** desired, int numInputs)
{
	Type** currentInput = new Type*[numLayers];
	bool condition = true;
	double previousError = 0;
	double currentError;
	int epochs = 1;
	
	Type** obtained;
	obtained = new Type*[numInputs];
	int numNeuronsLastLayer = this->layers[this->numLayers - 1].getNumNeurons();
	for (int i = 0; i < numInputs; i++)
		obtained[i] = new Type[numNeuronsLastLayer];

	while (condition)
	{

		for (int j = 0; j < numInputs; j++)
		{	
			
			Type* output;
			currentInput[0] = input[j];
			for (int i = 0; i < this->numLayers; i++)
			{
				Type* currentOutput = this->layers[i].answer(currentInput[i]);
				if (i == numLayers - 1)
					output = currentOutput;
				else
				{
					currentInput[i+1] = currentOutput;
				}

			}
			this->layers[this->numLayers - 1].adjustWeights(currentInput[this->numLayers - 1], desired[j], 0, 0, 0);
			for (int i = this->numLayers - 2; i >= 0; i--)
			{
				this->layers[i].adjustWeights(currentInput[i], desired[j], this->layers[i+1].getTransposedWeights(), this->layers[i+1].getDeltas(), this->layers[i+1].getNumNeurons());
			}

			for (int i = 0; i < numNeuronsLastLayer; i++)
				obtained[j][i] = output[i];
		}
		/** Calculando o erro medio */
		double mediumError = 0;
		for (int i = 0; i < numInputs; i++)
		{
			double localError = 0;
			for (int j = 0; j < numNeuronsLastLayer; j++)
			{
				localError += pow((desired[i][j] - obtained[i][j]),2);
			}
			localError /= 2;
			mediumError += localError;
		}
/*
		for (int i = 0; i < numInputs; i++)
		{
			cout << "Nova entrada: " << endl;
			for (int j = 0; j < numNeuronsLastLayer; j++)
				cout << desired[i][j] << " = " << obtained[i][j] << endl;
		}*/
		mediumError /= numInputs;
		/** pesos e condicao de parada */
		currentError = mediumError; //Calcular erro medio;
		if ( ((abs(currentError - previousError) < this->minimumError)) || (epochs >= this->maximumEpochs))
			condition = false;
		epochs += 1;
		previousError = currentError; // Calcular erro medio;
	}
	//cout << "treinei com " << numInputs << " instâncias" << endl;
	delete[] currentInput;

	for (int i = 0; i < numInputs; i++)
		delete[] obtained[i];
	delete[] obtained;
}

 // 

template <layerTemplate Layer, neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLP<Layer, Neuron, Activation, Type>::print()
{
	cout << "Num layers: " << this->numLayers << endl;
	for (int i = 0; i < this->numLayers; i++)
		this->layers[i].print();
}


#endif
