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
	tweetVectors = parseFile("trainData.csv");
	cout << tweetVectors.size();
	
	std::vector<std::string> posWords = tweetsToWords(tweetVectors,true);

	std::vector<std::string> negWords = tweetsToWords(tweetVectors, false);

/*	for (tweetStyle tweets : tweetVectors)
	{
		cout <<"Class : "<< tweets.clas<<'\t'
			<< "Date : " << tweets.date << '\t'
			<< "Id : " << tweets.id << '\t'
			<< "Message : " << tweets.message << '\t'
			<< "Query : " << tweets.query << '\n';
	}
	*/
	
/*	std::vector<std::string> strings;
//	pos = parseFileStrings("pos.txt");
//	neg = parseFileStrings("neg.txt");

//	cout << pos.size()<<'\t'<<neg.size()<<'\t';

	vocabulary.insert(std::end(vocabulary), std::begin(pos), std::end(pos));
	vocabulary.insert(std::end(vocabulary), std::begin(neg), std::end(neg));

	cout << vocabulary.size();
	*/

//	for (std::string str : strings)
//		cout << str << '\n';
//	pos = strings;

/*	std::vector<std::string> strings = parseTextToWords("@glovely  Thx for  the advice! We are just waiting for the Petosin to kick in ");

	for (std::string str : strings)
		cout << str << '\n';
		*/

	cout << "\n Words Size "<<posWords.size()<<'\t'<<negWords.size();
	cout << "\nPositive tweets " << johny::status.posWords << " negative tweets " << johny::status.negWords;
	getchar();
}