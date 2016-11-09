#ifndef __RBFNETWORK_H__
#define __RBFHNETWORK_H__

#include "../Common/normalize.cpp"
#include "../Common/network.h"
#include "../Common/templateDefinitions.h"


template < layerTemplate LayerHidden, 
			neuronTemplate NeuronHidden, 
			activationTemplate ActivationHidden,
			layerTemplate LayerOutput, 
			neuronTemplate NeuronOutput, 
			activationTemplate ActivationOutput,
			class Type >
			
class RBFNetwork : public Network<Type>
{
public:
	RBFNetwork(int, int, int);
	~RBFNetwork();
	void initialize(int, int, int);
	void train(Type**, Type** ,int);
	Type* answer(Type*);
	Type** answer(Type**);
	void setParameters(double, double, int, Type);
	void setWeightsHiddenNeuron(int, Type*);
	void setAccuracy(double){}
	void save(const char*);
	void load(const char*);
	double getAccuracy(){}
	
protected:
	LayerHidden < NeuronHidden, ActivationHidden, Type > hiddenLayer;
	LayerOutput < NeuronOutput, ActivationOutput, Type> outputLayer;
	int numWeightsInput;
	int numHiddenNeurons;
	int numOutputNeurons;
	double learningRate;
	double minimumError;
	int maximumEpochs;
	Type threshold;
};

template < layerTemplate LayerHidden,neuronTemplate NeuronHidden, activationTemplate ActivationHidden,
			layerTemplate LayerOutput, neuronTemplate NeuronOutput, activationTemplate ActivationOutput,
			class Type >
RBFNetwork<LayerHidden, NeuronHidden, ActivationHidden, LayerOutput, NeuronOutput, ActivationOutput, Type>::RBFNetwork(int numWeightsInput, int numHiddenNeurons , int numOutputNeurons)
{
	if ( (numWeightsInput != 0) && (numHiddenNeurons != 0) && (numOutputNeurons != 0) )
		initialize(numWeightsInput, numHiddenNeurons, numOutputNeurons);
}

template < layerTemplate LayerHidden,neuronTemplate NeuronHidden, activationTemplate ActivationHidden,
			layerTemplate LayerOutput, neuronTemplate NeuronOutput, activationTemplate ActivationOutput,
			class Type >
RBFNetwork<LayerHidden, NeuronHidden, ActivationHidden, LayerOutput, NeuronOutput, ActivationOutput, Type>::~RBFNetwork()
{

}

template < layerTemplate LayerHidden,neuronTemplate NeuronHidden, activationTemplate ActivationHidden,
			layerTemplate LayerOutput, neuronTemplate NeuronOutput, activationTemplate ActivationOutput,
			class Type >
void RBFNetwork<LayerHidden, NeuronHidden, ActivationHidden, LayerOutput, NeuronOutput, ActivationOutput, Type>::initialize(int numWeightsInput, int numHiddenNeurons , int numOutputNeurons)
{
	this->numWeightsInput = numWeightsInput;
	this->numHiddenNeurons = numHiddenNeurons;
	this->numOutputNeurons = numOutputNeurons;
	hiddenLayer.initialize(numHiddenNeurons, numWeightsInput);
	outputLayer.initialize(numOutputNeurons, numHiddenNeurons);
}


template < layerTemplate LayerHidden,neuronTemplate NeuronHidden, activationTemplate ActivationHidden,
			layerTemplate LayerOutput, neuronTemplate NeuronOutput, activationTemplate ActivationOutput,
			class Type >
void RBFNetwork<LayerHidden, NeuronHidden, ActivationHidden, LayerOutput, NeuronOutput, ActivationOutput, Type>::setParameters(double learningRate, double minimumError, int maximumEpochs, Type threshold)
{
	this->learningRate = learningRate;
	this->minimumError = minimumError;
	this->maximumEpochs = maximumEpochs;
	this->threshold = threshold;
	outputLayer.setMinimumError(minimumError);
	outputLayer.setLearningRate(learningRate);
	outputLayer.setMaximumEpochs(maximumEpochs);
	outputLayer.setThreshold(threshold);
}


template < layerTemplate LayerHidden,neuronTemplate NeuronHidden, activationTemplate ActivationHidden,
			layerTemplate LayerOutput, neuronTemplate NeuronOutput, activationTemplate ActivationOutput,
			class Type >
void RBFNetwork<LayerHidden, NeuronHidden, ActivationHidden, LayerOutput, NeuronOutput, ActivationOutput, Type>::setWeightsHiddenNeuron(int index, Type* weights)
{
	hiddenLayer.setWeightsNeuron(index, weights);
}




template < layerTemplate LayerHidden,neuronTemplate NeuronHidden, activationTemplate ActivationHidden,
			layerTemplate LayerOutput, neuronTemplate NeuronOutput, activationTemplate ActivationOutput,
			class Type >
void RBFNetwork<LayerHidden, NeuronHidden, ActivationHidden, LayerOutput, NeuronOutput, ActivationOutput, Type>::train(Type** inputSet, Type** intended, int numInputs)
{
	cout << "Treinando a camada hidden" << endl;
	hiddenLayer.train(inputSet, numInputs);
	//cout << "Treinou o hidden" << endl;
	Type** hiddenOutput;
	hiddenOutput = new Type* [numInputs];
	for (int i = 0; i < numInputs; i++)
		hiddenOutput[i] = new Type[this->numHiddenNeurons];
	
	for (int i = 0; i < numInputs; i++)
		hiddenOutput[i] = hiddenLayer.answer(inputSet[i]);
	
	//normalize(hiddenOutput, numInputs, this->numHiddenNeurons);
	
	/** Normalizar os vstores de entrada agora */

	cout << "Treinando a camada output" << endl;
	outputLayer.train(hiddenOutput, intended, numInputs);

	for (int i = 0; i < numInputs; i++)
		delete[] hiddenOutput[i];
	delete[] hiddenOutput;
}

template < layerTemplate LayerHidden,neuronTemplate NeuronHidden, activationTemplate ActivationHidden,
			layerTemplate LayerOutput, neuronTemplate NeuronOutput, activationTemplate ActivationOutput,
			class Type >
Type* RBFNetwork<LayerHidden, NeuronHidden, ActivationHidden, LayerOutput, NeuronOutput, ActivationOutput, Type>::answer(Type* input)
{
	//cout << "First answer" << endl;
	Type* hiddenOutput =  hiddenLayer.answer(input);

	cout << "Answer hidden" << endl;
	for (int i = 0; i < this->numHiddenNeurons; i++)
		cout << hiddenOutput[i] << " ";
	cout << endl;
	
	
	/** Normalizar os vetores de entrada agora */
	
	Type minimum = INFINITE, maximum = 0, diference;
	for (int j = 0; j < this->numHiddenNeurons; j++)
	{
		if (hiddenOutput[j] < minimum)
			minimum = hiddenOutput[j];
		if (hiddenOutput[j] > maximum)
			maximum = hiddenOutput[j];
	}
	diference = maximum - minimum;
	for (int j = 0; j < this->numHiddenNeurons; j++)
		hiddenOutput[j] = (hiddenOutput[j] - minimum) /diference;
	
	//outputLayer.print();
	
	
	Type* networkOutput = outputLayer.answer(hiddenOutput);
	
	/*cout << "saida" << endl;
	for (int i = 0; i < this->numOutputNeurons; i++)
		cout << networkOutput[i] << " ";
	cout << endl;*/
	

	delete[] hiddenOutput;
	return networkOutput;
}

template < layerTemplate LayerHidden,neuronTemplate NeuronHidden, activationTemplate ActivationHidden,
			layerTemplate LayerOutput, neuronTemplate NeuronOutput, activationTemplate ActivationOutput,
			class Type >
void RBFNetwork<LayerHidden, NeuronHidden, ActivationHidden, LayerOutput, NeuronOutput, ActivationOutput, Type>::save(const char* filename)
{
	cout << "Second Answer" << endl;
}


template < layerTemplate LayerHidden,neuronTemplate NeuronHidden, activationTemplate ActivationHidden,
			layerTemplate LayerOutput, neuronTemplate NeuronOutput, activationTemplate ActivationOutput,
			class Type >
void RBFNetwork<LayerHidden, NeuronHidden, ActivationHidden, LayerOutput, NeuronOutput, ActivationOutput, Type>::load(const char* filename)
{
	cout << "Second Answer" << endl;
}




#endif
