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


int main(int arglen, char *argc)
{
	using namespace johny;

	std::vector<tweetStyle> tweetVectors;
	tweetVectors = parseFile("data/trainingSample.csv");
//	cout << tweetVectors.size();

	
//	for (tweetStyle tweet : tweetVectors)
//		cout << tweet.message << '\t';

	johny::status.posProb = (double)johny::status.posWords / tweetVectors.size();
	johny::status.negProb = (double)johny::status.negWords / tweetVectors.size();
	
	std::vector<std::string> posTextWords = tweetsToWords(tweetVectors,true);

	std::vector<std::string> negTextWords = tweetsToWords(tweetVectors, false);


/*	for (tweetStyle tweets : tweetVectors)
	{
		cout <<"Class : "<< tweets.clas<<'\t'
			<< "Date : " << tweets.date << '\t'
			<< "Id : " << tweets.id << '\t'
			<< "Message : " << tweets.message << '\t'
			<< "Query : " << tweets.query << '\n';
	}
	*/
	
	std::vector<std::string> strings;
	pos = parseFileStrings("data/pos_red.txt");
	neg = parseFileStrings("data/neg_red.txt");

//	cout << pos.size()<<'\t'<<neg.size()<<'\t';

	vocabulary.insert(std::end(vocabulary), std::begin(pos), std::end(pos));

	vocabulary.insert(std::end(vocabulary), std::begin(neg), std::end(neg));


	cout << pos.size() << '\t' << neg.size();
	cout << '\n'<<vocabulary.size();
	
	

	

//	johny::findWordProb(posTextWords, vocab, vocabulary.size(), true);

//	johny::findWordProb(negTextWords, vocab, vocabulary.size(), false);

//	cout <<"\nVocabulary "<<vocab.word<< vocab.posProb << '\t' << vocab.negProb;
	
	// for target -

	vector<vocabStatus> vocabList;

	for (string newWord : vocabulary)
	{
		johny::vocabStatus vocab;

		vocab.word = newWord;

		vocabList.push_back(vocab);
	}


	johny::calculateProbOfWords(posTextWords, vocabList, vocabulary.size(), true);

	johny::calculateProbOfWords(negTextWords, vocabList, vocabulary.size(), false);

//	johny::calculateProbOfWords(posTextWords, )

/*	for (int i = 0; i < vocabulary.size(); i++)
	{
		johny::findWordProb(posTextWords, vocabList.at(i), vocabulary.size(), true);

	}

	for (int i = 0; i < vocabulary.size(); i++)
	{
//		johny::vocabStatus vocab;

//		vocab.word = vocabulary.at(i);

		johny::findWordProb(negTextWords, vocabList.at(i), vocabulary.size(), false);
	}

	*/

	for (int i = 0; i < vocabList.size();i++)
	cout <<endl<< vocabList.at(i).word << '\t' << vocabList.at(i).negProb <<'\t'<< vocabList.at(i).posProb;

	getchar();
}