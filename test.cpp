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

void charSeparator(char *c, int *indexes, unsigned int size)
{
	for (int i = 0; i < size; i++)
	{
		charLineHandler(c + i*300, indexes + i*20, i);
	}
}



int maint()
{

	std::vector<johny::tweetStyle> tweetVectors;
	tweetVectors = johnyGPU::parseFileCuda("data/trainingSample.csv",true);

	std::cout << "done";

	for (int i = 0; i < tweetVectors.size(); i++)
		std::cout << tweetVectors[i].message<<'\n';
		
	
	getchar();

	return 0;
}