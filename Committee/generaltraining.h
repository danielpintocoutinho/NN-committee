#ifndef __GENERALTRAINING_H__
#define __GENERALTRAINING_H__

#include <vector>
#include <tuple>

#include "../network.h"

template <class Type>
class GeneralTraining
{
public:
	void train(Type**, Type**, int, int, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	Type** answer(Type*, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	int* cluster(Type*, int, vector<Network<Type>*>,vector < tuple <vector<Network<Type>*>, bool, bool>>);
	int getNumAnswers();
	int getNumClusters();
private:
	int numAnswers;
	int numClusters;
	int numClasses;
};


template <class Type>
void GeneralTraining<Type>::train(Type** input, Type** desired, int numInputs, int numAttributes, int numClasses, vector<Network<Type>*> networks,
	 vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{
	int numNetworks = networks.size();
	this->numClasses  = numClasses;
	vector <vector<int>> dependences;
	vector < tuple< vector<Type*>, vector< int > > > trainingInput;
	vector < vector<Type*> > desiredInput;
	for (auto &network: networks)
	{
		vector<int> newDependence;
		dependences.push_back(newDependence);

		tuple<vector<Type*>, vector<int>> newTrainingInput;
		trainingInput.push_back(newTrainingInput);

		vector<Type*> newDesiredInput;
		desiredInput.push_back(newDesiredInput);

		/* Training input here */
	}


	tuple <bool, bool> trained[numNetworks]; // First element means it is able to be trained, the second means if it already has been trained
	for (int i = 0; i < numNetworks; i++)
	{
		bool isDependent = false;
		for (int j = 0;  j < numNetworks; j++)
		{
			
			for (auto &searchedNetwork: get<0>(connections[j]) )
			{
				cout << networks[i] << " == " << searchedNetwork << " = " << (networks[i] == searchedNetwork)  << endl;
				if (networks[i] == searchedNetwork)
				{
					dependences[i].push_back(j);
					get<1>(trainingInput[j]).push_back(i);
					get<0>(trained[i]) = false;
					get<1>(trained[i]) = false;
					isDependent = true;
				}
			}
		}
		if (isDependent == false)
			{
				cout << "Rede " << i << " não é dependente de ninguém" << endl;
				get<0>(trained[i]) = true;
				get<1>(trained[i]) = false;
			}
	}




	for (int i = 0; i < numNetworks; i++)
	{
		if (get<1>(connections[i]) == true)
		{
			//Inserting input in training input
			for (int j = 0; j < numInputs; j++)
			{
				get<0>(trainingInput[i]).push_back(input[j]);
				desiredInput[i].push_back(desired[j]);
			}
		}
	}


	bool condition = true;
	int it = 1;
	while (condition == true)
	{
		bool changed = false;
		for (int i = 0; i < numNetworks; i++)
		{
			
			if (get<0>(trained[i]) == true && get<1>(trained[i]) == false)
			{
				int numTrainingInputs = get<0>(trainingInput[i]).size();
				Type** trainingInputAux = new Type* [numTrainingInputs];
				Type** desiredInputAux = new Type*[numTrainingInputs];
				for (int i = 0; i < numTrainingInputs; i++)
				{
					trainingInputAux[i] = new Type[numAttributes];
					desiredInputAux[i] = new Type[numClasses];
				}
				cout << "Treinando rede: " << i << endl;


				for (int j = 0; j < numTrainingInputs; j++)
				{
					for (int k = 0; k < numAttributes; k++)
					{
						trainingInputAux[j][k] = get<0>(trainingInput[i])[j][k];
					}
					for (int k = 0; k < numClasses; k++)
					{
						vector < vector<Type*> > desiredInput;
						desiredInputAux[j][k] = desiredInput[i][j][k];
					}
				}

				networks[i]->train(trainingInputAux, desiredInputAux, numTrainingInputs);
				
				/*TODO HERE --- Include the results in the corresponding networks*/
				
				for (int j = 0; j < numTrainingInputs; j++)
				{
					Type* answer = networks[i]->answer(trainingInputAux[j]);
					for (int k = 0; k < get<1>(trainingInput[i]).size(); i++)
					{
						int indexNetwork = get<1>(trainingInput[i])[k];
						get<0>(trainingInput[indexNetwork]).push_back(answer);
						desiredInput[indexNetwork].push_back(desiredInput[i][j]);
					}
					delete[] answer;
				}

				/* TODOING */

				for (int i = 0; i < numTrainingInputs; i++)
				{
					delete[] trainingInputAux[i];
					delete[] desiredInputAux[i];
				}
				delete[] trainingInputAux;
				delete[] desiredInputAux;

				get<1>(trained[i]) = true;
				changed = true;

			}
			else
			{
				if (dependences[i].size() != 0)
				{
					for (int indice = 0; indice < dependences[i].size(); indice++)
						if (get<1>(trained[dependences[i][indice]]) == true)
						{
							dependences[i].erase(dependences[i].begin() + indice);
							indice--;
						}
				}
				if (dependences[i].size() == 0)
					get<0>(trained[i]) = true;
			}

		}
		if (changed == false)
			condition = false;

		cout << "Chegou aqui " << it++ << "?" << endl;
	}



	for (int i = 0; i < trainingInput.size(); i++)
	{
		int numTrainingInputs = get<0>(trainingInput[i]).size();
		for (int j = 0; j < numTrainingInputs; j++)
		{
			Type* currentInput = get<0>(trainingInput[i])[j];
			if (currentInput != 0)
			{
				delete[] currentInput;
				currentInput = 0;
			}
		}
	}
}



template <class Type>
Type** GeneralTraining<Type>::answer(Type* input, int numAttributes, vector<Network<Type>*> networks,
	vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{
	this->numNetworksOutput = 0;
	int numNetworks = networks.size();
	vector <vector<int>> dependences;
	vector < tuple< vector<Type*>, vector< int > > > answeringInput;
	vector <Type*> answers;
	for (auto &network: networks)
	{
		vector<int> newDependence;
		dependences.push_back(newDependence);

		tuple<vector<Type*>, vector<int>> newAnsweringInput;
		answeringInput.push_back(newAnsweringInput);

		/* Training input here */
	}


	tuple <bool, bool> answered[numNetworks]; // First element means it is able to answer, the second means if it already has answered
	for (int i = 0; i < numNetworks; i++)
	{
		bool isDependent = false;
		for (int j = 0;  j < numNetworks; j++)
		{
			
			for (auto &searchedNetwork: get<0>(connections[j]) )
			{
				cout << networks[i] << " == " << searchedNetwork << " = " << (networks[i] == searchedNetwork)  << endl;
				if (networks[i] == searchedNetwork)
				{
					dependences[i].push_back(j);
					get<1>(answeringInput[j]).push_back(i);
					get<0>(answered[i]) = false;
					get<1>(answered[i]) = false;
					isDependent = true;
				}
			}
		}
		if (isDependent == false)
			{
				cout << "Rede " << i << " não é dependente de ninguém" << endl;
				get<0>(answered[i]) = true;
				get<1>(answered[i]) = false;
			}
	}




	for (int i = 0; i < numNetworks; i++)
	{
		if (get<1>(connections[i]) == true)
		{
			get<0>(answeringInput[i]).push_back(input);
		}
	}


	bool condition = true;
	int it = 1;
	while (condition == true)
	{
		bool changed = false;
		for (int i = 0; i < numNetworks; i++)
		{
			
			if (get<0>(answered[i]) == true && get<1>(answered[i]) == false)
			{
				int numAnsweringInputs = get<0>(answeringInput[i]).size();
				Type** AnsweringInputAux = new Type* [numAnsweringInputs];
				for (int i = 0; i < numAnsweringInputs; i++)
				{
					AnsweringInputAux[i] = new Type[numAttributes];
				}
				cout << "Treinando rede: " << i << endl;


				for (int j = 0; j < numAnsweringInputs; j++)
				{
					for (int k = 0; k < numAttributes; k++)
					{
						AnsweringInputAux[j][k] = get<0>(answeringInput[i])[j][k];
					}
				}
				
				/*TODO HERE --- Include the results in the corresponding networks*/
				
				for (int j = 0; j < numAnsweringInputs; j++)
				{
					Type* answer = networks[i]->answer(AnsweringInputAux[j]);
					for (int k = 0; k < get<1>(answeringInput[i]).size(); i++)
					{
						int indexNetwork = get<1>(answeringInput[i])[k];
						get<0>(answeringInput[indexNetwork]).push_back(answer);
					}
					if (get<2>(connections[i]) == true)
					{
						answers.push_back(answer);
						this->numNetworksOutput++;
					}
				}

				/* TODOING */

				for (int i = 0; i < numAnsweringInputs; i++)
				{
					delete[] AnsweringInputAux[i];
				}
				delete[] AnsweringInputAux;

				get<1>(answered[i]) = true;
				changed = true;

			}
			else
			{
				if (dependences[i].size() != 0)
				{
					for (int indice = 0; indice < dependences[i].size(); indice++)
						if (get<1>(answered[dependences[i][indice]]) == true)
						{
							dependences[i].erase(dependences[i].begin() + indice);
							indice--;
						}
				}
				if (dependences[i].size() == 0)
					get<0>(answered[i]) = true;
			}

		}
		if (changed == false)
			condition = false;

		cout << "Chegou aqui " << it++ << "?" << endl;
	}

	for (int i = 0; i < answeringInput.size(); i++)
	{
		int numAnsweringInputs = get<0>(answeringInput[i]).size();
		for (int j = 0; j < numAnsweringInputs; j++)
		{
			Type* currentInput = get<0>(answeringInput[i])[j];
			if (currentInput != 0)
			{
				delete[] currentInput;
				currentInput = 0;
			}
		}
	}


	this->numAnswers = answers.size();
	Type** answersAux = new Type*[numAnswers];
	for (int i = 0; i < numAnswers; i++)
		answersAux[i] = new Type[numClasses];
	for (int i = 0; i < numAnswers; i++)
		for (int j = 0; j < numClasses; j++)
			answersAux[i][j] = answers[i][j];

	return answersAux;	
}

template <class Type>
int* GeneralTraining<Type>::cluster(Type* input, int numAttributes, vector<Network<Type>*> networks,
	vector < tuple <vector<Network<Type>*>, bool, bool>> connections)
{
	this->numNetworksOutput = 0;
	int numNetworks = networks.size();
	vector <vector<int>> dependences;
	vector < tuple< vector<Type*>, vector< int > > > answeringInput;
	vector <int> clusters;
	for (auto &network: networks)
	{
		vector<int> newDependence;
		dependences.push_back(newDependence);

		tuple<vector<Type*>, vector<int>> newAnsweringInput;
		answeringInput.push_back(newAnsweringInput);

		/* Training input here */
	}


	tuple <bool, bool> answered[numNetworks]; // First element means it is able to answer, the second means if it already has answered
	for (int i = 0; i < numNetworks; i++)
	{
		bool isDependent = false;
		for (int j = 0;  j < numNetworks; j++)
		{
			
			for (auto &searchedNetwork: get<0>(connections[j]) )
			{
				cout << networks[i] << " == " << searchedNetwork << " = " << (networks[i] == searchedNetwork)  << endl;
				if (networks[i] == searchedNetwork)
				{
					dependences[i].push_back(j);
					get<1>(answeringInput[j]).push_back(i);
					get<0>(answered[i]) = false;
					get<1>(answered[i]) = false;
					isDependent = true;
				}
			}
		}
		if (isDependent == false)
			{
				cout << "Rede " << i << " não é dependente de ninguém" << endl;
				get<0>(answered[i]) = true;
				get<1>(answered[i]) = false;
			}
	}




	for (int i = 0; i < numNetworks; i++)
	{
		if (get<1>(connections[i]) == true)
		{
			get<0>(answeringInput[i]).push_back(input);
		}
	}


	bool condition = true;
	int it = 1;
	while (condition == true)
	{
		bool changed = false;
		for (int i = 0; i < numNetworks; i++)
		{
			
			if (get<0>(answered[i]) == true && get<1>(answered[i]) == false)
			{
				int numAnsweringInputs = get<0>(answeringInput[i]).size();
				Type** AnsweringInputAux = new Type* [numAnsweringInputs];
				for (int i = 0; i < numAnsweringInputs; i++)
				{
					AnsweringInputAux[i] = new Type[numAttributes];
				}
				cout << "Treinando rede: " << i << endl;


				for (int j = 0; j < numAnsweringInputs; j++)
				{
					for (int k = 0; k < numAttributes; k++)
					{
						AnsweringInputAux[j][k] = get<0>(answeringInput[i])[j][k];
					}
				}
				
				/*TODO HERE --- Include the results in the corresponding networks*/
				
				for (int j = 0; j < numAnsweringInputs; j++)
				{
					if (get<2>(connections[i]) == true)
					{
						int resultCluster = networks[i]->cluster(AnsweringInputAux[j]);
						clusters.push_back(resultCluster);
						this->numNetworksOutput++;
					}
					else
					{
						Type* answer = networks[i]->answer(AnsweringInputAux[j]);
						for (int k = 0; k < get<1>(answeringInput[i]).size(); i++)
						{
							int indexNetwork = get<1>(answeringInput[i])[k];
							get<0>(answeringInput[indexNetwork]).push_back(answer);
						}
					}
				}

				/* TODOING */

				for (int i = 0; i < numAnsweringInputs; i++)
				{
					delete[] AnsweringInputAux[i];
				}
				delete[] AnsweringInputAux;

				get<1>(answered[i]) = true;
				changed = true;

			}
			else
			{
				if (dependences[i].size() != 0)
				{
					for (int indice = 0; indice < dependences[i].size(); indice++)
						if (get<1>(answered[dependences[i][indice]]) == true)
						{
							dependences[i].erase(dependences[i].begin() + indice);
							indice--;
						}
				}
				if (dependences[i].size() == 0)
					get<0>(answered[i]) = true;
			}

		}
		if (changed == false)
			condition = false;

		cout << "Chegou aqui " << it++ << "?" << endl;
	}

	for (int i = 0; i < answeringInput.size(); i++)
	{
		int numAnsweringInputs = get<0>(answeringInput[i]).size();
		for (int j = 0; j < numAnsweringInputs; j++)
		{
			Type* currentInput = get<0>(answeringInput[i])[j];
			if (currentInput != 0)
			{
				delete[] currentInput;
				currentInput = 0;
			}
		}
	}


	this->numClusters = clusters.size();
	int* clustersAux = new int[numClusters];
	for (int i = 0; i < numClusters; i++)
		clustersAux[i] = clusters[i];

	return clustersAux;
}

template <class Type>
int GeneralTraining<Type>::getNumAnswers()
{
	return this->numAnswers;
}

template <class Type>
int GeneralTraining<Type>::getNumClusters()
{
	return this->numClusters;
}




#endif