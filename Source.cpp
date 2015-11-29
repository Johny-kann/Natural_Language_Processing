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
	tweetVectors = parseFile("trainingSample.csv");
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
	pos = parseFileStrings("pos.txt");
	neg = parseFileStrings("neg.txt");

//	cout << pos.size()<<'\t'<<neg.size()<<'\t';

	vocabulary.insert(std::end(vocabulary), std::begin(pos), std::end(pos));

	vocabulary.insert(std::end(vocabulary), std::begin(neg), std::end(neg));


	cout << pos.size() << '\t' << neg.size();
	cout << '\n'<<vocabulary.size();
	
	johny::vocabStatus vocab;

	vocab.word = vocabulary.at(0);

	johny::findWordProb(posTextWords, vocab, vocabulary.size(), true);

	johny::findWordProb(negTextWords, vocab, vocabulary.size(), false);

	cout <<"\nVocabulary "<<vocab.word<< vocab.posProb << '\t' << vocab.negProb;
	
//	for (std::string str : strings)
//		cout << str << '\n';
//	pos = strings;

/*	std::vector<std::string> strings = parseTextToWords("@glovely  Thx for  the advice! We are just waiting for the Petosin to kick in ");

	for (std::string str : strings)
		cout << str << '\n';
		*/

/*	cout << "\n Words Size "<<posTextWords.size()<<'\t'<<negTextWords.size();
	cout << "\nPositive tweets " << johny::status.posWords << " negative tweets " << johny::status.negWords;
	cout << johny::status.posProb << " " << johny::status.negProb;
	*/
	getchar();
}