#include <string>
#include <stdio.h>
#include <vector>
#include "models.h"
#include <iostream>
#include  <fstream>
#include <sstream>
#include <iomanip>

struct cudaString2
{
	char *str;
	int length;
};

struct myString
{
	char *str;
	short index[10];
	short totIndexes;
};

void separate(char *c, int length, myString *temp)
{
	char *prev, *prev2, *curr;
	temp->str = c;
	temp->index[0] = 0;
	temp->totIndexes = 1;

	for (int i = 2; i < length; i++)
	{
		curr = &c[i];
		prev = &c[i - 1];	
		prev2 = &c[i - 2];

		if (*curr == '\"' && *prev == ',' && *prev2 == '\"')
		{

			c[i] = '\0';

			c[i-1] = '\0';

			c[i-2] = '\0';
		
		}
		else if (*prev == '\0')
		{
			temp->index[temp->totIndexes] = i;
			temp->totIndexes++;
		}
	}

	c[0] = '\0';
	temp->index[0] = 1;
	c[length-1] = '\0';
	
}

void tester2(cudaString2 &str)
{
	//	cudaString str;
	std::string string = "Hello";
	str.str = new char[string.size() + 1];

	memcpy(str.str, string.data(), string.size()+1);
	//	str.str = string.data();
	str.length = string.size();
	//printf(str.str);
}

void print2(cudaString2 str)
{
	printf(str.str);
}


std::vector<johny::tweetStyle> parseFile(std::string fileName)
{
	using namespace std;
	using namespace johny;

	ifstream file(fileName); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	string value;

	vector<tweetStyle> tweetVectors;
	//	std::vector<std::string>   result;
	string line;
	std::vector<string> lines;

	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

//	cout << "\nLines pushed " << lines.size();

	cudaString2 *linesCuda = new cudaString2[lines.size()];

	for (int i = 0; i < lines.size(); i++)
	{
		linesCuda[i].str = &lines[i][0];
		linesCuda[i].length = lines[i].size();
		linesCuda[i].str[lines[i].size()] = '\0';
	}

//	cout << "\nCuda Lines assigned " << lines.size();

	myString *tweetsCuda = new myString[lines.size()];

	for (int i = 0; i < lines.size();i++)
	{
	separate(linesCuda[i].str, linesCuda[i].length, &tweetsCuda[i]);
	}

//	int d = lines.size();
//	cout << "\nseparated " << lines.size();

	for (int i = 0; i < lines.size(); i++)
	{
		johny::tweetStyle tweet;
		tweet.clas = tweetsCuda[i].str + tweetsCuda[i].index[0];
		tweet.id = tweetsCuda[i].str + tweetsCuda[i].index[1];
		tweet.date = tweetsCuda[i].str + tweetsCuda[i].index[2];
		tweet.query = tweetsCuda[i].str + tweetsCuda[i].index[3];
		tweet.sender = tweetsCuda[i].str + tweetsCuda[i].index[4];
		tweet.message = tweetsCuda[i].str + tweetsCuda[i].index[5];

		tweetVectors.push_back(tweet);
	}
	file.close();
	return tweetVectors;
}


int main()
{

	std::vector<johny::tweetStyle> tweetVectors;
	tweetVectors = parseFile("data/trainingSample.csv");

	for (johny::tweetStyle tweet : tweetVectors)
	{
		std::cout << tweet.message<<'\n';
	}
	getchar();
}