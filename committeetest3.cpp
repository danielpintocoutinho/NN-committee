#include "Committee/committee.h"
#include "Committee/averagecombiner.h"
#include "Committee/simpletraining.h"
#include "Committee/generaltraining.h"
#include "Committee/generalvoting.h"
#include "Committee/proximitycombiner.h"
#include "Committee/threevotingcombiner.h"
#include "Committee/threefilteringtraining.h"
#include "Committee/clusteringtraining.h"

#include "Common/normalize.cpp"
#include "Common/logisticactivationfunction.h"
#include "Common/euclidiandistance.h"

#include "MLP/mlplayer.h"
#include "MLP/mlpneuron.h"
#include "MLP/mlp.h"

#include "KohonenLight/kohonenneuron.h"
#include "KohonenLight/gaussianneighborhood.h"
#include "KohonenLight/kohonen.h"

#include "Input/input.cpp"
#include "Output/output.cpp"



int main()
{
	srand(time(NULL));
	ifstream data("iris.data");
	int numInputs, numClasses, numAttributes;
	data >> numInputs >> numClasses >> numAttributes;
	/*data >> numInputs;
	numClasses = 3;
	numAttributes = 4;*/

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

	int numLines = 3, numColumns = 1, neighborhoodSize = 0;
	double learningRate = 0.2;
	int sizeMlpSet = numLines * numColumns;

	Kohonen< KohonenNeuron, GaussianNeighborhoodFunction, EuclidianDistance, double> kohonen1(numLines, numColumns, numAttributes);
	Kohonen< KohonenNeuron, GaussianNeighborhoodFunction, EuclidianDistance, double> kohonen2(numLines, numColumns, numAttributes);
	Kohonen< KohonenNeuron, GaussianNeighborhoodFunction, EuclidianDistance, double> kohonen3(numLines, numColumns, numAttributes);
	kohonen1.setParameters(learningRate, neighborhoodSize);
	kohonen2.setParameters(learningRate, neighborhoodSize);
	kohonen3.setParameters(learningRate, neighborhoodSize);

	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double>* mlpSet1 = 
		new MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double>[sizeMlpSet];
	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double>* mlpSet2 = 
		new MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double>[sizeMlpSet];
	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double>* mlpSet3 = 
		new MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double>[sizeMlpSet];

	for (int i = 0; i < sizeMlpSet ; i++)
	{
		mlpSet1[i].initialize(2, numAttributes, {4, numClasses});
		mlpSet1[i].setParameters(learningRate, 0.000001, 10000, 0);

		mlpSet2[i].initialize(2, numAttributes, {4, numClasses});
		mlpSet2[i].setParameters(learningRate, 0.000001, 10000, 0);

		mlpSet3[i].initialize(2, numAttributes, {4, numClasses});
		mlpSet3[i].setParameters(learningRate, 0.000001, 10000, 0);
	}

	Committee<ClusteringTraining, GeneralCombiner, double> test3;
	test3.setNumAttributes(numAttributes);
	test3.setNumClasses(numClasses);
	test3.useStandardAnswerMode(false);

	test3.add(&kohonen1, true, false);
	test3.add(&kohonen2, true, false);
	test3.add(&kohonen3, true, false);
	for (int i = 0; i < sizeMlpSet; i++)
	{
		test3.add(&mlpSet1[i], false);
		test3.add(&mlpSet2[i], false);
		test3.add(&mlpSet3[i], false);
	}
	for (int i = 0; i < sizeMlpSet ; i++)
	{
		test3.connect(&kohonen1, &mlpSet1[i]);
		test3.connect(&kohonen2, &mlpSet2[i]);
		test3.connect(&kohonen3, &mlpSet3[i]);
	}

	test3.train(input, intended,numInputs);
	
	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double> mlp1(2, numAttributes, {4, numClasses});
	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double> mlp2(2, numAttributes, {4, numClasses});
	MLP<MLPLayer, MLPNeuron, LogisticActivationFunction, double> mlp3(2, numAttributes, {4, numClasses});

	mlp1.setParameters(learningRate, 0.000001, 10000, 0);
	mlp2.setParameters(learningRate, 0.000001, 10000, 0);
	mlp3.setParameters(learningRate, 0.000001, 10000, 0);

	//cout << "Numero de exemplos das individuais: " << (numInputs) << endl;

	mlp1.train(input, intended, (numInputs)/3);
	mlp2.train(input, intended, (numInputs)/3);
	mlp3.train(input, intended, (numInputs)/3);


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
	
	//cout << "leu tudo" << endl;

	for (int i = 0; i < numInputsTest; i++)
	{
		result[i] = test3.answer(inputTest[i]);
		result1[i] = mlp1.answer(inputTest[i]);
		result2[i] = mlp2.answer(inputTest[i]);
		result3[i] = mlp3.answer(inputTest[i]);
	}


	//cout << endl << "CHEGOU AQUI CHEGOU AQUI CHEGOU AQUI" << endl << endl;
/*
	for (int i = 0; i < numInputsTest; i++)
	{

		result1[i] = test3.answerIndividually(inputTest[i], 1);
		result2[i] = test3.answerIndividually(inputTest[i], 2);
		result3[i] = test3.answerIndividually(inputTest[i], 3);
	}
*/
	//cout << "NORMALIZANDO NORMALIZANDO" << endl << endl;

	

	Output<double> outputinho;
	Output<double> outputinho1;
	Output<double> outputinho2;
	Output<double> outputinho3;

	normalize(result, numInputsTest, numClasses);
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

	//cout << "OUTPUTINHO OUTPUTINHO OUTPUTINHO" << endl << endl << endl << endl;

	outputinho.analyzeResult(intendedTest, result, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho.accuracy() << endl;
	cout <<  outputinho.accuracy() << endl;

	
	outputinho1.analyzeResult(intendedTest, result1, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho1.accuracy() << endl;

	outputinho2.analyzeResult(intendedTest, result2, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho2.accuracy() << endl;

	outputinho3.analyzeResult(intendedTest, result3, numInputsTest, numClasses);
	//cout << endl << endl << "Acuracia: " << outputinho3.accuracy() << endl;

	double mediaIndividuais;
	mediaIndividuais = (outputinho1.accuracy() + outputinho2.accuracy() + outputinho3.accuracy())/ 3; 
	//cout << "Média das 3 individuais " << mediaIndividuais << endl;
	cout <<  mediaIndividuais << endl;

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
	}

	delete[] intendedTest;
	delete[] inputTest;
	delete[] result;
	delete[] result1;
	delete[] result2;
	delete[] result3;


	return 0;
}
