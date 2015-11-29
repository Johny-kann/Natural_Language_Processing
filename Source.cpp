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


int main2(int arglen, char *argc)
{
	using namespace johny;

	std::vector<tweetStyle> tweetVectors,tweetTargets;
	tweetVectors = parseFile("data/trainingSample.csv");
	tweetTargets = parseFileTarget("data/testSample.csv");
//	cout << tweetVectors.size();

	
//	for (tweetStyle tweet : tweetVectors)
//		cout << tweet.message << '\t';

	johny::status.posProb = (double)johny::status.posWords / tweetVectors.size();
	johny::status.negProb = (double)johny::status.negWords / tweetVectors.size();
	
	std::vector<std::string> posTextWords = tweetsToWords(tweetVectors,true);

	std::vector<std::string> negTextWords = tweetsToWords(tweetVectors, false);


	std::vector<std::string> strings;
	pos = parseFileStrings("data/pos_red.txt");
	neg = parseFileStrings("data/neg_red.txt");

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


	johny::calculateProbOfWords(posTextWords, vocabList, vocabulary.size(), true);

	johny::calculateProbOfWords(negTextWords, vocabList, vocabulary.size(), false);

/*	for (int i = 0; i < vocabList.size();i++)
	cout <<endl<< vocabList.at(i).word << '\t' << vocabList.at(i).negProb <<'\t'<< vocabList.at(i).posProb;
	*/

	cout << "\n..........Prob Calcuated.............\n";

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