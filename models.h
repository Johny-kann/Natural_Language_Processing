#include <string>
#include <vector>
#include <list>

#ifndef MODELS_H
#define MODELS_H

namespace johny
{

struct tweetStyle
{
	std::string clas, id, date, query, sender, message;
};

struct dataStatus
{
	int posWords, negWords;
	double posProb, negProb;
};

struct vocabStatus
{
	std::string word;
	double posProb, negProb;
};

extern std::vector<std::string> pos, neg, vocabulary;

extern dataStatus status;


//extern list<std::string> tot;
}
#endif