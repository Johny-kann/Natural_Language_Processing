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
	std::vector<tweetStyle> tweetVectors;
	tweetVectors = parseFile("testdata.csv");

	cout << tweetVectors.size();
/*	for (int i = 0; i<tweetVectors.size(); i++)
		cout << tweetVectors.at(i).clas<<'\t'
			<<tweetVectors.at(i).date<<'\t'
			<<tweetVectors.at(i).id<<'\t'
			<< tweetVectors.at(i).query << '\t'
	<< tweetVectors.at(i).message << '\n';
	*/		
	
//	std::vector<std::string> strings;
	pos = parseFileStrings("pos.txt");
	neg = parseFileStrings("neg.txt");

	cout << pos.size()<<'\t'<<neg.size();

//	for (std::string str : strings)
//		cout << str << '\n';
//	pos = strings;
	getchar();
}