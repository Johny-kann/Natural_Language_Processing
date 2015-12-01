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

int main(int arglen, char **argc)
{
	using namespace johny;

//	for (int i = 0; i < arglen; i++)
//		printf("%s", argc[i]);

	std::string trainingSet(argc[1]);

	std::string testingSet(argc[2]);

	std::string posWords(argc[3]);

	std::string negWords(argc[4]);

	cout << "Training Set " << trainingSet << "\nTesting Set " << testingSet<<std::endl;

	cout << "Pos Words " << posWords << "\nNeg Words " << negWords << std::endl;


	if (!fileExists(trainingSet))
	{
		cout << "File doesn't exists";
		return(-2);
	}

	if (!fileExists(testingSet))
	{
		cout << "File doesn't exists";
		return(-2);
	}

	if (!fileExists(posWords))
	{
		cout << "File doesn't exists";
		return(-2);
	}

	if (!fileExists(negWords))
	{
		cout << "File doesn't exists";
		return(-2);
	}

	std::vector<tweetStyle> tweetVectors,tweetTargets;
	tweetVectors = johnyGPU::parseFileCuda(trainingSet
//		"data/trainingSample.csv"
											,true);
	tweetTargets = johnyGPU::parseFileCuda(testingSet
//		"data/testSample.csv"
											,false);
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
	cout << "Tweet to words done\n";


	std::vector<std::string> strings;
	pos = parseFileStrings(posWords
//			"data/pos_red.txt"
		);
	neg = parseFileStrings(negWords
//		"data/neg_red.txt"
		);

//	cout << pos.size()<<'\t'<<neg.size()<<'\t';

	vocabulary.insert(std::end(vocabulary), std::begin(pos), std::end(pos));

	vocabulary.insert(std::end(vocabulary), std::begin(neg), std::end(neg));


	cout << pos.size() << '\t' << neg.size();
	cout << '\n'<<vocabulary.size();
	

	vector<vocabStatus> vocabList;

	for (string newWord : vocabulary)
	{
		johny::vocabStatus vocab;

		vocab.word = newWord;

		vocabList.push_back(vocab);
	}

	cout << "Positive Prob calculation starts\n";

	johny::calculateProbOfWords(posTextWords, vocabList, vocabulary.size(), true);

	cout << "Negative Prob calculation starts\n";

	johny::calculateProbOfWords(negTextWords, vocabList, vocabulary.size(), false);

/*	for (int i = 0; i < vocabList.size();i++)
	cout <<endl<< vocabList.at(i).word << '\t' << vocabList.at(i).negProb <<'\t'<< vocabList.at(i).posProb;
	*/

	cout << "\n..........Prob Calcuated.............\n";

	cout << "............Testing Using Samples.......\n";

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
			tweetTargets[i].clas = "4";
			probs.push_back(probPos);
		}
		else
		{
			tweetTargets[i].clas = "0";
			probs.push_back(probNeg);
		}


	}

	

	for (int i = 0; i < tweetTargets.size(); i++)
	{
		cout << "\n" << tweetTargets[i].message << '\t' << probs[i]<<'\t'<<tweetTargets[i].clas;
	}

	getchar();

	return 0;
}