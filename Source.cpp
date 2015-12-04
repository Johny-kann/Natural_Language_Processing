#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>
#include "logics.h"
//#include <math.h>


using namespace std;

inline bool fileExists(const std::string& name) {
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

int mainCPU(int arglen, char **argc);
int mainGPU(int arglen, char **argc);

int main(int arglen, char **argc)
{
	if (arglen > 3)
	{
		std::string naiveBayes(argc[1]);

		if (naiveBayes.compare("naive-bayes-cpu")==0)
		{
			std::cout << "\nchoosing Naive GPU and CPU mode";
			mainCPU(arglen, argc);
		}
		else
		{	
			std::cout << "\nchoosing Normal GPU mode";
			mainGPU(arglen, argc);
		}
	}
	else
	{
		std::cout << "\nchoosing Normal GPU mode";
		mainGPU(arglen, argc);
	}
	return 0;
}

int mainGPU(int arglen, char **argc)
{
	using namespace johny;

	std::string trainingSet;

	std::string posWords;

	std::string negWords;

	if (arglen > 2)
	{
		trainingSet = argc[1];

		posWords = argc[2];

		negWords = argc[3];
	}
	else
	{
		trainingSet = "data/testdata.csv";
	
		posWords = "data/pos.txt";

		negWords = "data/neg.txt";
	}

	std::cout << "Pos Words " << posWords << "\nNeg Words " << negWords << std::endl;

	if (!fileExists(trainingSet))
	{
		std::cout << trainingSet << " File doesn't exists";
		
		return(-2);
	}

	if (!fileExists(posWords))
	{
		std::cout << posWords << " File doesn't exists";
		
		return(-2);
	}

	if (!fileExists(negWords))
	{
		std::cout << negWords << "File doesn't exists";
		
		return(-2);
	}


	std::vector<tweetStyle> tweetVectors;
	tweetVectors = johnyGPU::parseFileCuda(trainingSet, true);

	char *posWordsArray , *negWordsArray;

	int posWordSize = johny::parseFileStrings(posWords, &posWordsArray);

	int negWordSize = johny::parseFileStrings(negWords, &negWordsArray);

	std::cout << "\nNumber of Positive and Negative words" << posWordSize << '\t' << negWordSize;


	johnyGPU::tweetsProbs(tweetVectors, posWordsArray, posWordSize, negWordsArray, negWordSize);
	
	for (int i = 0; i < tweetVectors.size(); i++)
	{
		std::cout << "\n" << tweetVectors[i].message << '\t' << tweetVectors[i].prob;
	}

	delete[] posWordsArray;
	delete[] negWordsArray;

//	getchar();

	return 0;
}

int mainCPU(int arglen, char **argc)
{
	using namespace johny;


	std::string trainingSet(argc[2]);

	std::string testingSet(argc[3]);

	std::string posWords(argc[4]);

	std::string negWords(argc[5]);

	std::cout << "Training Set " << trainingSet << "\nTesting Set " << testingSet << std::endl;

	std::cout << "Pos Words " << posWords << "\nNeg Words " << negWords << std::endl;


	if (!fileExists(trainingSet))
	{
		std::cout << trainingSet << " File doesn't exists";
		return(-2);
	}

	if (!fileExists(testingSet))
	{
		std::cout << testingSet << " File doesn't exists";
		return(-2);
	}

	if (!fileExists(posWords))
	{
		std::cout << posWords << " File doesn't exists";
		return(-2);
	}

	if (!fileExists(negWords))
	{
		std::cout << negWords << " File doesn't exists";
		return(-2);
	}

	std::vector<tweetStyle> tweetVectors,tweetTargets;

	tweetVectors = johnyGPU::parseFileCuda(trainingSet,true);
	tweetTargets = johnyGPU::parseFileCuda(testingSet,false);
//	cout << tweetVectors.size();

	
//	for (tweetStyle tweet : tweetVectors)
//		cout << tweet.message << '\t';

	johny::status.posProb = (double)johny::status.posWords / tweetVectors.size();
	johny::status.negProb = (double)johny::status.negWords / tweetVectors.size();
	
	std::vector<std::string> posTextWords;
//	 = tweetsToWords(tweetVectors, true);

	std::vector<std::string> negTextWords;
//	 = tweetsToWords(tweetVectors, false);

	johnyGPU::tweetsToWordsGPU(tweetVectors, posTextWords, negTextWords);

//	johny::tweetsToWords(tweetVectors, posTextWords, negTextWords);
	std::cout << "Tweet to words done\n";


	std::vector<std::string> strings;
	pos = parseFileStrings(posWords);
	
	neg = parseFileStrings(negWords);

//	cout << pos.size()<<'\t'<<neg.size()<<'\t';

	vocabulary.insert(std::end(vocabulary), std::begin(pos), std::end(pos));

	vocabulary.insert(std::end(vocabulary), std::begin(neg), std::end(neg));


	std::cout << pos.size() << '\t' << neg.size();
	std::cout << '\n' << vocabulary.size();
	

	vector<vocabStatus> vocabList;

	for (string newWord : vocabulary)
	{
		johny::vocabStatus vocab;

		vocab.word = newWord;

		vocabList.push_back(vocab);
	}

	std::cout << "Positive Prob calculation starts\n";

	johny::calculateProbOfWords(posTextWords, vocabList, vocabulary.size(), true);

	std::cout << "Negative Prob calculation starts\n";

	johny::calculateProbOfWords(negTextWords, vocabList, vocabulary.size(), false);

	std::cout << "\n..........Prob Calcuated.............\n";

	std::cout << "............Testing Using Samples.......\n";

	vector<double> probs;
	for (int i = 0; i < tweetTargets.size(); i++)
	{
	//	vector<string> strings = parseTextToWords(tweetTargets.at(i).message);
		double probPos = showtweetProb(
			parseTextToWords(tweetTargets[i].message), vocabList, true);

		double probNeg = showtweetProb(
			parseTextToWords(tweetTargets[i].message), vocabList, false);

		if (probPos > probNeg)
		{
			tweetTargets[i].clas = "1";
			probs.push_back(probPos);
		}
		else
		{
			tweetTargets[i].clas = "-1";
			probs.push_back(probNeg);
		}


	}

	for (int i = 0; i < tweetTargets.size(); i++)
	{
		std::cout << "\n" << tweetTargets[i].message << '\t' << probs[i] << '\t' << tweetTargets[i].clas;
	}

	return 0;
}