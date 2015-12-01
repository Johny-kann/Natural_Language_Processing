#include <string>
#include <stdio.h>
#include <vector>
#include "models.h"
#include <iostream>
#include  <fstream>
#include <sstream>
#include <iomanip>
#include "reductionCuda.cuh"
#include "logics.h"



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

void wordHandler(char *c, int *indexes, int lineNum)
{
	char prev = '0', prev2, curr;
	indexes[0] = 150 * lineNum;

	int count = 0;

	for (int i = 0; c[i] != '\0'; i++)
	{
		curr = c[i];
		if (i!=0)
			prev = c[i - 1];

		if ( !((curr >= 'A' && curr <= 'Z') || (curr >= 'a' && curr <= 'z') || (curr >= '0' && curr <= '9')))
		{
			c[i] = '\0';
			curr = '\0';
		}
		
		if (prev == '\0' && curr !='\0')
		{
			count++;
			indexes[count] = 150 * lineNum + i;
		}
	}

	count++;
	indexes[count] = -1;
}

void charSeparator(char *c, int *indexes, unsigned int size)
{
	for (int i = 0; i < size; i++)
	{
		charLineHandler(c + i*300, indexes + i*70, i);
	}
}


void tweetToWordsGPU(std::vector<johny::tweetStyle> tweets,std::vector<std::string> &posWords, std::vector<std::string> &negWords)
{	
	using namespace std;

	char *message = new char[150*tweets.size()];
	char *clas = new char[5 * tweets.size()];
	
	int *index = new int[70*tweets.size()];

	for (int i = 0; i < tweets.size();i++)
	{
		memcpy(&clas[5 * i], tweets[i].clas.c_str(), sizeof(char)*tweets[i].clas.size());
		clas[5 * i + tweets[i].clas.size()] = '\0';
	
		memcpy(&message[150 * i], tweets[i].message.c_str(), sizeof(char)*tweets[i].message.size());
		message[150 * i + tweets[i].message.size()] = '\0';
	}

	for (int i = 0; i < tweets.size(); i++)
	{
//		std::cout << &message[150 * i]<<'\n';
			wordHandler(&message[150*i], &index[70*i], i);
	}

/*	for (int i = 0; i < tweets.size(); i++)
	{
		std::cout << '\n';
		for (int j = 0; index[70*i+j] != -1 ;j++)
			std::cout << index[70*i+j] << ' ';
		
	}
	*/

	for (int i = 0; i < tweets.size(); i++)
	{
		int flag;
		if (clas[5 * i] == '0')
			flag = -1;
		else if (clas[5 * i] == '4')
			flag = 1;
		else if (clas[5 * i] == '2')
			flag = 0;

		for (int j = 0; index[70 * i + j] != -1; j++)
		{
			std::string str;
			str.assign(&message[index[70 * i + j]]);

			if (flag == -1)
				negWords.push_back(str);
			else if (flag == 1)
				posWords.push_back(str);
		}
	}

	delete[] message;
	delete[] index;
	delete[] clas;

//	return text_words;
}


int maint()
{

	std::vector<johny::tweetStyle> tweetVectors;
	tweetVectors = johnyGPU::parseFileCuda("data/trainSample2.csv",true);

//	for (int i = 0; i < tweetVectors.size(); i++)
//		std::cout << tweetVectors[i].message << '\n';

//	char chars[300] = "Hello How Do you do? Awwww.....";
//	int *index = new int[20 * 1];

//	wordHandler(chars, index, 0);

//	std::cout << "done";

	std::vector < std::string> posW, negW;
	tweetToWordsGPU(tweetVectors, posW, negW);

/*	for (int i = 0; index[i] != -1; i++)
		std::cout << index[i] << '\n';

	for (int i = 0; index[i] != -1; i++)
	{
		std::string str;
		str = &chars[index[i]];
		strings.push_back(str);
	}
	*/
	for (std::string str : posW)
	{
		std::cout << str << '\n';
	}
	for (std::string str : negW)
	{
		std::cout << str << '\n';
	}
//	for (int i = 0; i < tweetVectors.size(); i++)
//		std::cout << tweetVectors[i].message<<'\n';
		
	
	getchar();

	return 0;
}