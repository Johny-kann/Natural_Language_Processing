#include <string>
#include <stdio.h>
#include <vector>
#include "models.h"
#include <iostream>
#include  <fstream>
#include <sstream>
#include <iomanip>
#include "reductionCuda.cuh"

void separate(cudaString *cuda
	, myString *temp)
{
	char *prev, *prev2, *curr;
	temp->str = cuda->str;
	char *c = cuda->str;
	temp->index[0] = 0;
	temp->totIndexes = 1;

	for (int i = 2; i < cuda->length; i++)
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
	c[cuda->length-1] = '\0';
	
}

void tester2(cudaString &str)
{
	//	cudaString str;
	std::string string = "Hello";
	str.str = new char[string.size() + 1];

	memcpy(str.str, string.data(), string.size()+1);
	//	str.str = string.data();
	str.length = string.size();
	//printf(str.str);
}

void print2(cudaString str)
{
	printf(str.str);
}

void charLineHandler(char *c, int *indexes,int lineNum)
{
	char *prev, *prev2, *curr;
	indexes[0] = 300*lineNum;

	int count = 0;

	for (int i = 2; c[i]!='\0'; i++)
	{
		curr = &c[i];
		prev = &c[i - 1];
		prev2 = &c[i - 2];

		if (*curr == '\"' && *prev == ',' && *prev2 == '\"')
		{
			c[i] = '\0';
			c[i - 1] = '\0';
			c[i - 2] = '\0';

		}
		else if (*prev == '\0')
		{
			count++;
			indexes[count] = 300 * lineNum + i;
		}
	}


	c[0] = '\0';
	indexes[0] = 300 * lineNum+1;
	count++;
	indexes[count] = -1;

}

void charSeparator(char *c, int *indexes, unsigned int size)
{
	for (int i = 0; i < size; i++)
	{
		charLineHandler(c + i*300, indexes + i*20, i);
	}
}


std::vector<johny::tweetStyle> parseFile(std::string fileName)
{
	using namespace std;
	using namespace johny;

	ifstream file(fileName); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	string value;

	vector<tweetStyle> tweetVectors;

	string line;
	std::vector<string> lines;

	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

	char *charLines = new char[lines.size() * 300];
	int *indexes = new int[lines.size() * 20];

	for (int i = 0; i < lines.size(); i++)
	{
		for (int j = 0; j < lines[i].size()+1; j++)
		{ 
			if (j == lines[i].size())
				charLines[i * 300 + j] = '\0';
			else
				charLines[i * 300 + j] = lines[i][j];
				
		}

	}

	charSeparator(charLines, indexes, lines.size());


	for (int i = 0; i < lines.size(); i++)
	{
		johny::tweetStyle tweet;
		(tweet).clas = &charLines[indexes[20 * i]];

		(tweet).id = &charLines[indexes[20 * i + 1]];

		(tweet).date = &charLines[indexes[20 * i + 2]];

		(tweet).query = &charLines[indexes[20 * i + 3]];

		(tweet).sender = &charLines[indexes[20 * i + 4]];

		(tweet).message = &charLines[indexes[20 * i + 5]];


		tweetVectors.push_back(tweet);
	}

/*	for (int i = 0; i < lines.size(); i++)
	{
		johny::tweetStyle tweet;
		tweet.clas = charLines[300 * i] + indexes[20 * i];
			//tweetsCuda[i].str + tweetsCuda[i].index[0];
		tweet.id = charLines[300 * i] + indexes[20 * i + 1];
		//	tweetsCuda[i].str + tweetsCuda[i].index[1];
		tweet.date = charLines[300 * i] + indexes[20 * i + 2];
		//	tweetsCuda[i].str + tweetsCuda[i].index[2];
		tweet.query = charLines[300 * i] + indexes[20 * i + 3];
		//	tweetsCuda[i].str + tweetsCuda[i].index[3];
		tweet.sender = charLines[300 * i] + indexes[20 * i + 4];
		//	tweetsCuda[i].str + tweetsCuda[i].index[4];
		tweet.message = charLines[300 * i] + indexes[20 * i + 5];
		//	tweetsCuda[i].str + tweetsCuda[i].index[5];

		tweetVectors.push_back(tweet);
	}
	*/
	file.close();
	return tweetVectors;
}


int main()
{

	std::vector<johny::tweetStyle> tweetVectors;
	tweetVectors = parseFile("data/trainingSample.csv");

	for (int i = 0; i < tweetVectors.size(); i++)
		std::cout << tweetVectors[i].message<<'\n';
		
	getchar();
}