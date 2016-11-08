#ifndef __COMMITTEE_H__
#define __COMMITTEE_H__

#include <iostream>
#include <vector>
#include <tuple>

#include "../network.h"
#include "../templateDefinitions.h"

using namespace std;

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
class Committee
{
public:
	Committee();
	~Committee();
	void add(Network<Type>*, bool, bool);
	void add(Network<Type>**, int, bool, bool);
	void add(initializer_list<Network<Type>*>, bool, bool);
	void print();
	void printConnections();
	void connect(Network<Type>*, Network<Type>*);
	void connect(Network<Type>*, Network<Type>**, int);
	void connect(Network<Type>*, initializer_list<Network<Type>*>);
	void train(Type**, Type**, int);
	void setNumAttributes(int);
	void setNumClasses(int);
	void setNumTrainingExamples(int);
	void useStandardAnswerMode(bool);
	Type* answer(Type*);
	Type* answerIndividually(Type*, int);
	int cluster(Type*);
private:
	vector<Network<Type>*> networks;
	vector < tuple <vector<Network<Type>*>, bool, bool>> connections; // First tells if the network is connected to the input and second if it is connected to the output
	
	Combiner<Type> combiner;
	Training<Type> training;
	int numNetworks;
	int numAttributes;
	int numClasses;
	int getIndex(Network<Type>*);
	bool useStandardMode;
};

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
Committee<Training, Combiner, Type>::Committee()
{
	this->numNetworks = 0;
	this->useStandardMode = false;
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
Committee<Training, Combiner, Type>::~Committee()
{
	if (numNetworks != 0)
	{
		networks.clear();
		connections.clear();
	}	
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::add(Network<Type>* newNetwork, bool connectedToInput = true, bool connectedToOutput = true)
{
	networks.push_back(newNetwork);

	vector <Network<Type>*> newConnection;
	connections.push_back( make_tuple(newConnection, connectedToInput, connectedToOutput));
	numNetworks += 1;
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::add(Network<Type>** newNetworks, int numNewNetworks, bool connectedToInput = true, bool connectedToOutput = true)
{
	for (int i = 0; i < numNewNetworks; i++)
	{
		networks.push_back(newNetworks[i]);
		vector <Network<Type>*> newConnection;
		connections.push_back( make_tuple(newConnection, connectedToInput, connectedToOutput));
	}
	numNetworks += numNewNetworks;
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::add(initializer_list<Network<Type>*> newNetworks, bool connectedToInput = true, bool connectedToOutput = true)
{
	for (auto &newNetwork: newNetworks)
	{
		networks.push_back(newNetwork);
		vector <Network<Type>*> newConnection;
		connections.push_back( make_tuple(newConnection, connectedToInput, connectedToOutput));
		numNetworks++;
	}
	
}



template <trainingTemplate Training, combinerTemplate Combiner, class Type>
int Committee<Training, Combiner, Type>::getIndex(Network<Type>* network)
{
	for (int i = 0; i < networks.size(); i++)
		if (networks[i] == network)
			return i;
	return -1;
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::connect(Network<Type>* inputNetwork, Network<Type>* outputNetwork)
{
	int indexInputNetwork = getIndex(inputNetwork);
	if (indexInputNetwork != -1)
		get<0>(connections[indexInputNetwork]).push_back(outputNetwork);
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::connect(Network<Type>* inputNetwork, Network<Type>** outputNetworks, int numNetworks)
{
	int indexInputNetwork = getIndex(inputNetwork);
	if (indexInputNetwork != -1)
		for (int i = 0; i < numNetworks; i++)
			get<0>(connections[indexInputNetwork]).push_back(outputNetworks[i]);
	
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::connect(Network<Type>* inputNetwork, initializer_list<Network<Type>*> outputNetworks)
{
	int indexInputNetwork = getIndex(inputNetwork);
	if (indexInputNetwork != -1)
		for (auto &outputNetwork: outputNetworks)
			get<0>(connections[indexInputNetwork]).push_back(outputNetwork);
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::print()
{
	cout << "Print" << endl;
	for (int i = 0; i < networks.size(); i++)
		cout << networks[i] << endl;
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::printConnections()
{
	cout << "Print Connections" << endl;
	for (int i = 0; i < connections.size(); i++)
	{
		cout << "Network: " << i << endl;
		for (int j = 0; j < get<0>(connections[i]).size(); j++)
			cout << get<0>(connections[i])[j] << endl;
	}
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::setNumClasses(int numClasses)
{
	this->numClasses = numClasses;
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::setNumAttributes(int numAttributes)
{
	this->numAttributes = numAttributes;
}


template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::train(Type** input, Type** desired, int numInputs)
{
	training.train(input, desired, numInputs, numAttributes, numClasses, networks, connections);
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
Type* Committee<Training, Combiner, Type>::answer(Type* input)
{


	Type** answers;
	int numAnswers;
	Type* networkOutput = new Type[numClasses];
	if (useStandardMode == false)
	{
		answers = training.answer(input, this->numAttributes, this->networks, this->connections);
		numAnswers = training.getNumAnswers();
	}
	else 
	{
		answers = new Type*[this->numNetworks];
		numAnswers = this->numNetworks;
		for (int i = 0; i < networks.size(); i++)
		{
			answers[i] = this->networks[i]->answer(input);

		}
	}
	Type* networkOutputAux = combiner.answer(answers, numAnswers, this->numClasses);
	for (int i = 0; i < numClasses; i++)
		networkOutput[i] = networkOutputAux[i];
	for (int i = 0; i < numAnswers; i++)
		delete[] answers[i];
	delete[] answers;
	delete[] networkOutputAux;
	return networkOutput;
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
int Committee<Training, Combiner, Type>::cluster(Type* input)
{
	int *clusters = training.cluster(input, this->numAttributes, this->networks, this->connections);
	int numClusters = training.getNumClusters();
	int resultClusters = combiner.cluster(clusters, numClusters);
	delete[] clusters;
	return resultClusters;
}



template <trainingTemplate Training, combinerTemplate Combiner, class Type>
Type* Committee<Training, Combiner, Type>::answerIndividually(Type* input, int index)
{
	Type* networkOutput = networks[index-1]->answer(input);
	return networkOutput;
}


template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::setNumTrainingExamples(int numExamples)
{
	training.setNumExamples(numExamples);
}

template <trainingTemplate Training, combinerTemplate Combiner, class Type>
void Committee<Training, Combiner, Type>::useStandardAnswerMode(bool useStandard)
{
	this->useStandardMode = useStandard;
}


#endif