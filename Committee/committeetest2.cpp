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
#include "proximitycombiner.h"
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


	double learningRate = 0.3;

	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double> mlp1(2, numAttributes, {4, numClasses});
	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double> mlp2(2, numAttributes, {4, numClasses});
	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double> mlp3(2, numAttributes, {4, numClasses});

	mlp1.setParameters(learningRate, 0.000001, 10000, 0);
	mlp2.setParameters(learningRate, 0.000001, 10000, 0);
	mlp3.setParameters(learningRate, 0.000001, 10000, 0);

	Committee<ThreeFilteringTraining, ThreeVotingCombiner, double> test2;
	test2.setNumTrainingExamples(15);

	test2.add(&mlp1);
	test2.add(&mlp2);
	test2.add(&mlp3);

	test2.setNumAttributes(numAttributes);
	test2.setNumClasses(numClasses);

	test2.train(input, intended,numInputs);
	

	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double> mlp4(2, numAttributes, {4,  numClasses});
	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double> mlp5(2, numAttributes, {4, numClasses});
	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double> mlp6(2, numAttributes, {4,  numClasses});

	mlp4.setParameters(learningRate, 0.000001, 10000, 0);
	mlp5.setParameters(learningRate, 0.000001, 10000, 0);
	mlp6.setParameters(learningRate, 0.000001, 10000, 0);
	

	//cout << "Treinar individualmente" << endl;
	mlp4.train(input, intended, 15);
	mlp5.train(input, intended, 15);
	mlp6.train(input, intended, 15);


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
	double** result4;
	result4 = new double*[numInputsTest];
	double** result5;
	result5 = new double*[numInputsTest];
	double** result6;
	result6 = new double*[numInputsTest];

	for (int i = 0; i < numInputsTest; i++)
	{
		inputTest[i] = new double[numAttributes];
		intendedTest[i] = new double[numClasses];
		result[i] = new double[numClasses];
		result1[i] = new double[numClasses];
		result2[i] = new double[numClasses];
		result3[i] = new double[numClasses];
		result4[i] = new double[numClasses];
		result5[i] = new double[numClasses];
		result6[i] = new double[numClasses];
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


	for (int i = 0; i < numInputsTest; i++)
	{
		result[i] = test2.answer(inputTest[i]);
		result1[i] = test2.answerIndividually(inputTest[i], 1);
		result2[i] = test2.answerIndividually(inputTest[i], 2);
		result3[i] = test2.answerIndividually(inputTest[i], 3);
		result4[i] = mlp4.answer(inputTest[i]);
		result5[i] = mlp5.answer(inputTest[i]);
		result6[i] = mlp6.answer(inputTest[i]);
	}




	Output<double> outputinho;
	Output<double> outputinho1;
	Output<double> outputinho2;
	Output<double> outputinho3;
	Output<double> outputinho4;
	Output<double> outputinho5;
	Output<double> outputinho6;


	normalize(result, numInputsTest, numClasses);
	normalize(result1, numInputsTest, numClasses);
	normalize(result2, numInputsTest, numClasses);
	normalize(result3, numInputsTest, numClasses);
	normalize(result4, numInputsTest, numClasses);
	normalize(result5, numInputsTest, numClasses);
	normalize(result6, numInputsTest, numClasses);
/*
	cout << "DESIRED DESIRED DESIRED" << endl;

	for (int i = 0; i < numInputsTest; i++)
	{
		for (int j = 0; j < numClasses; j++)
			cout << intendedTest[i][j] << " ";
		cout << endl;
	}

	cout << endl << "RESULT RESULT RESULT" << endl;
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

	//cout << "OUTPUTINHO OUTPUTINHO OUTPUTINHO" << endl << endl << endl << endl;

	outputinho.analyzeResult(intendedTest, result, numInputsTest, numClasses);
//	cout << endl << endl << "Acuracia: " << outputinho.accuracy() << endl;
	cout << outputinho.accuracy() << endl;
	
	outputinho1.analyzeResult(intendedTest, result1, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho1.accuracy() << endl;

	outputinho2.analyzeResult(intendedTest, result2, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho2.accuracy() << endl;

	outputinho3.analyzeResult(intendedTest, result3, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho3.accuracy() << endl;

	outputinho4.analyzeResult(intendedTest, result4, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho4.accuracy() << endl;

	outputinho5.analyzeResult(intendedTest, result5, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho5.accuracy() << endl;

	outputinho6.analyzeResult(intendedTest, result6, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho6.accuracy() << endl;

	double mediaComite, mediaIndividuais;
	mediaComite = (outputinho1.accuracy() + outputinho2.accuracy() + outputinho3.accuracy()) / 3;
	mediaIndividuais = (outputinho4.accuracy() + outputinho5.accuracy() + outputinho6.accuracy()) / 3; 
	//cout << "Média das 3 do comitê " << mediaComite << endl;
	//cout << "Média das 3 individuais " << mediaIndividuais << endl;
	cout << mediaComite << endl;
	cout << mediaIndividuais << endl;


	data.close();
	test.close();

	for (int i = 0; i < numInputs; i++)
	{
		delete[] input[i];
		delete[] intended[i];
	}
	delete[] intended;
	delete[] input;

	
	for (int i = 0; i < numInputsTest; i++)
	{
		delete[] inputTest[i];
		delete[] intendedTest[i];
		delete[] result[i];
		delete[] result1[i];
		delete[] result2[i];
		delete[] result3[i];
		delete[] result4[i];
		delete[] result5[i];
		delete[] result6[i];
	}

	delete[] intendedTest;
	delete[] inputTest;
	delete[] result;
	delete[] result1;
	delete[] result2;
	delete[] result3;
	delete[] result4;
	delete[] result5;
	delete[] result6;

	return 0;
}
