#ifndef __MLPLAYER_H__
#define __MLPLAYER_H__

#include "../Common/templateDefinitions.h"

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
class MLPLayer
{
public:
	MLPLayer(int, int);
	~MLPLayer();
	void initialize(int, int);
	Type* answer(Type*);
	void adjustWeights(Type*, Type*, Type**, Type*, int);
	void setLearningRate(double);
	void setMinimumError(double);
	void setMaximumEpochs(int);
	void setThreshold(Type);
	void setParameters(double, double, int, Type);
	void setOutputLayer();
	int getNumNeurons();
	Type** getTransposedWeights();
	Type* getDeltas();
	Type* getOutput();
	void print();

private:
	Neuron<Activation, Type>* neurons;
	Type* output;
	bool outputLayer;
	int numNeurons;
	int numWeights;
	double learningRate;
	double minimumError;
	int maximumEpochs;
};

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
MLPLayer<Neuron, Activation, Type>::MLPLayer(int numNeurons = 0, int numWeights = 0)
{
	this->numNeurons = 0;
	if (numNeurons != 0)
		initialize(numNeurons, numWeights);
}

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
MLPLayer<Neuron, Activation, Type>::~MLPLayer()
{
	if (this->numNeurons != 0)
	{
		delete[] this->neurons;
		delete[] this->output;
	}
}


template <neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLPLayer<Neuron, Activation, Type>::initialize(int numNeurons, int numWeights)
{
	this->numNeurons = numNeurons;
	this->numWeights = numWeights;
	this->neurons = new Neuron<Activation, Type>[numNeurons];
	this->output = new Type[numNeurons];
	this->outputLayer = false;
	for (int i = 0; i < numNeurons; i++)
		this->neurons[i].initialize(numWeights);
}

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
int MLPLayer<Neuron, Activation, Type>::getNumNeurons()
{
	return this->numNeurons;
}

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLPLayer<Neuron,Activation, Type>::setLearningRate(double newLearningRate)
{
	this->learningRate = newLearningRate;
}

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLPLayer<Neuron, Activation, Type>::setMinimumError(double newMinimumError)
{
	this->minimumError = newMinimumError;
}

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLPLayer<Neuron, Activation, Type>::setMaximumEpochs(int newMaximumEpochs)
{
	this->maximumEpochs = newMaximumEpochs;
}

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLPLayer<Neuron,Activation,Type>::setThreshold(Type threshold)
{
	for (int i = 0; i < this->numNeurons; i++)
		this->neurons[i].setThreshold(threshold);
}

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLPLayer<Neuron,Activation,Type>::setParameters(double newLearningRate, double newMinimumError, int newMaximumEpochs, Type threshold)
{
	this->learningRate = newLearningRate;
	this->minimumError = newMinimumError;
	this->maximumEpochs = newMaximumEpochs;
	for (int i = 0; i < this->numNeurons; i++)
		this->neurons[i].setThreshold(threshold);
}

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLPLayer<Neuron,Activation,Type>::setOutputLayer()
{
	this->outputLayer = true;
}



template <neuronTemplate Neuron, activationTemplate Activation, class Type>
Type** MLPLayer<Neuron, Activation, Type>::getTransposedWeights()
{
	Type** transposedWeights = new Type*[numWeights];
	for (int i = 0; i < numWeights; i++)
		transposedWeights[i] = new Type[numNeurons];

	for (int i = 0; i < numNeurons; i++)
	{
		const Type* auxiliaryWeights = this->neurons[i].getWeights();
		for (int j = 0; j < numWeights; j++)
			transposedWeights[j][i] = auxiliaryWeights[j];
	}

	return transposedWeights;
}


template <neuronTemplate Neuron, activationTemplate Activation, class Type>
Type* MLPLayer<Neuron, Activation, Type>::getDeltas()
{
	Type* deltas = new Type[numNeurons];
	for (int i = 0; i < numNeurons; i++)
		deltas[i] = this->neurons[i].getDelta();
	return deltas;
}


template <neuronTemplate Neuron, activationTemplate Activation, class Type>
Type* MLPLayer<Neuron, Activation, Type>::answer(Type* input)
{
	for (int i = 0; i < this->numNeurons; i++)
		this->output[i] = this->neurons[i].activationFunction(input);
	return this->output;
}

template <neuronTemplate Neuron, activationTemplate Activation, class Type>
Type* MLPLayer<Neuron, Activation, Type>::getOutput()
{
	return this->output;
}


template <neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLPLayer<Neuron, Activation, Type>::adjustWeights(Type* input, Type* desired, Type** nextLayerWeights, Type* nextLayerDeltas, int numNeuronsNextLayer)
{
	for (int i = 0; i < this->numNeurons; i++)
	{
		if (this->outputLayer)
		{
			this->neurons[i].adjustWeights(input, desired[i], this->learningRate, i);
		}
		else 
			this->neurons[i].adjustWeights(input, this->learningRate, nextLayerWeights[i], nextLayerDeltas, numNeuronsNextLayer);
	}
}


template <neuronTemplate Neuron, activationTemplate Activation, class Type>
void MLPLayer<Neuron, Activation, Type>::print()
{
	for (int i = 0; i < this->numNeurons; i++)
		this->neurons[i].print();
	cout << " _________ " << endl;
}


#endif
