#include "committee.h"


#include "../Kohonen/euclidiandistance.h"
#include "../Kohonen/kohonenneuron.h"
#include "../Kohonen/gaussianneighborhood.h"
#include "../Kohonen/kohonen.h"

#include "../RBF/gaussianactivationfunction.h"
#include "../RBF/linearactivationfunction.h"
#include "../RBF/rbfhiddenneuron.h"
#include "../RBF/rbfoutputneuron.h"
#include "../RBF/rbfhiddenlayer.h"
#include "../RBF/rbfoutputlayer.h"
#include "../RBF/rbfnetwork.h"

#include "../MLP/logisticactivationfunction.h"
#include "../MLP/mlplayer.h"
#include "../MLP/mlpneuron.h"
#include "../MLP/mlp.h"


#include "../Input/input.cpp"
#include "../Output/output.cpp"

#include "averagecombiner.h"
#include "simpletraining.h"
#include "generaltraining.h"
#include "generalvoting.h"
#include "threevotingcombiner.h"
#include "threefilteringtraining.h"
#include "clusteringtraining.h"

#include "../normalize.cpp"


int main()
{
	srand(time(NULL));
	ifstream data("iris.data");
	int numInputs, numClasses, numAttributes;
	data >> numInputs >> numClasses >> numAttributes;
	
	double** input;
	input = new double*[numInputs];
	double** intended;
	intended = new double*[numInputs];
	for (int i = 0; i < numInputs; i++)
	{
		input[i] = new double[numAttributes];
		intended[i] = new double[numClasses];
	}
	
	for (int i = 0; i < numInputs; i++)
	{
		for (int j = 0; j < numAttributes; j++)
			data >> input[i][j];
		for (int j = 0; j < numClasses; j++)
			data >> intended[i][j];
	}
	
	
	//Normalizar o input
	
	normalize(input, numInputs, numAttributes);

	RBFNetwork <RBFHiddenLayer, RBFHiddenNeuron, GaussianActivationFunction, 
				RBFOutputLayer, RBFOutputNeuron, LinearActivationFunction, double > rbf1(numAttributes,numClasses,numClasses);

	RBFNetwork <RBFHiddenLayer, RBFHiddenNeuron, GaussianActivationFunction, 
				RBFOutputLayer, RBFOutputNeuron, LinearActivationFunction, double > rbf2(numAttributes,numClasses,numClasses);

	RBFNetwork <RBFHiddenLayer, RBFHiddenNeuron, GaussianActivationFunction, 
				RBFOutputLayer, RBFOutputNeuron, LinearActivationFunction, double > rbf3(numAttributes,numClasses,numClasses);
				
	rbf1.setParameters(0.5, 0.000001, 10000, 0.5);
	rbf2.setParameters(0.5, 0.000001, 10000, 0.5);
	rbf3.setParameters(0.5, 0.000001, 10000, 0.5);


	Committee<SimpleTraining, AverageCombiner, double> test1;

	test1.add(&rbf1);
	test1.add(&rbf2);
	test1.add(&rbf3);

	test1.setNumAttributes(numAttributes);
	test1.setNumClasses(numClasses);

	test1.train(input, intended,numInputs);
	
	ifstream test("iris.test");
	int numInputsTest;
	test >> numInputsTest;
	

	double** inputTest;
	inputTest = new double*[numInputsTest];
	double** intendedTest;
	intendedTest = new double*[numInputsTest];
	double** result;
	result = new double*[numInputsTest];


	double** result1;
	result1 = new double*[numInputsTest];
	double** result2;
	result2 = new double*[numInputsTest];
	double** result3;
	result3 = new double*[numInputsTest];
	for (int i = 0; i < numInputsTest; i++)
	{
		inputTest[i] = new double[numAttributes];
		intendedTest[i] = new double[numClasses];
		result[i] = new double[numClasses];
		result1[i] = new double[numClasses];
		result2[i] = new double[numClasses];
		result3[i] = new double[numClasses];
	}
	



	for (int i = 0; i < numInputsTest; i++)
	{
		for (int j = 0; j < numAttributes; j++)
			test >> inputTest[i][j];
		for (int j = 0; j < numClasses; j++)
			test >> intendedTest[i][j];
	}

	//Normalizar o input de teste
	normalize(inputTest, numInputsTest, numAttributes);
	
	cout << "leu tudo" << endl;

	for (int i = 0; i < numInputsTest; i++)
	{
		result[i] = test1.answer(inputTest[i]);
	}


	cout << endl << "CHEGOU AQUI CHEGOU AQUI CHEGOU AQUI" << endl << endl;

	for (int i = 0; i < numInputsTest; i++)
	{

		result1[i] = test1.answerIndividually(inputTest[i], 1);
		result2[i] = test1.answerIndividually(inputTest[i], 2);
		result3[i] = test1.answerIndividually(inputTest[i], 3);
	}

	cout << "NORMALIZANDO NORMALIZANDO" << endl << endl;

	normalize(result, numInputsTest, numClasses);

	Output<double> outputinho1;
	Output<double> outputinho2;
	Output<double> outputinho3;


	normalize(result1, numInputsTest, numClasses);
	normalize(result2, numInputsTest, numClasses);
	normalize(result3, numInputsTest, numClasses);


/*
	cout << "NORMALIZE NORMALIZE NORMALIZE" << endl;
	cout << endl  << endl << endl;


	for (int i = 0; i < numInputsTest; i++)
	{
		for (int j = 0; j < numClasses; j++)
			cout << result[i][j] << " ";
		cout << endl;
	}

	cout << "RESULT RESULT RESULT - 1" << endl;
	cout << endl  << endl << endl;


	for (int i = 0; i < numInputsTest; i++)
	{
		for (int j = 0; j < numClasses; j++)
			cout << result1[i][j] << " ";
		cout << endl;
	}

	cout << "RESULT RESULT RESULT - 2" << endl;
	cout << endl  << endl << endl;


	for (int i = 0; i < numInputsTest; i++)
	{
		for (int j = 0; j < numClasses; j++)
			cout << result2[i][j] << " ";
		cout << endl;
	}


	cout << "RESULT RESULT RESULT - 3" << endl;
	cout << endl  << endl << endl;


	for (int i = 0; i < numInputsTest; i++)
	{
		for (int j = 0; j < numClasses; j++)
			cout << result3[i][j] << " ";
		cout << endl;
	}*/

	cout << "OUTPUTINHO OUTPUTINHO OUTPUTINHO" << endl << endl << endl << endl;

	outputinho.analyzeResult(intendedTest, result, numInputsTest, numClasses);
	cout << endl << endl << "Acuracia: " << outputinho.accuracy() << endl;

	
	outputinho1.analyzeResult(intendedTest, result1, numInputsTest, numClasses);
	cout << endl << endl << "Acuracia: " << outputinho1.accuracy() << endl;

	outputinho2.analyzeResult(intendedTest, result2, numInputsTest, numClasses);
	cout << endl << endl << "Acuracia: " << outputinho2.accuracy() << endl;

	outputinho3.analyzeResult(intendedTest, result3, numInputsTest, numClasses);
	cout << endl << endl << "Acuracia: " << outputinho3.accuracy() << endl;

	//rbf.answer(inputTest);
	
	
	cout << "Chega aqui" << endl;
	for (int i = 0; i < numInputs; i++)
	{
		delete[] input[i];
		delete[] intended[i];
	}
	delete[] intended;
	delete[] input;
	
	cout << "Chega aqui" << endl;
	for (int i = 0; i < numInputsTest; i++)
	{
		delete[] inputTest[i];
		delete[] intendedTest[i];
		delete[] result[i];
		delete[] result1[i];
		delete[] result2[i];
		delete[] result3[i];
	}
	cout << "Chega aqui" << endl;
	delete[] intendedTest;
	delete[] inputTest;
	delete[] result;
	delete[] result1;
	delete[] result2;
	delete[] result3;


	return 0;
}
