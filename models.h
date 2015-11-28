#include <string>
#include <vector>
#include <list>

#ifndef MODELS_H
#define MODELS_H

struct tweetStyle
{
	std::string clas, id, date, query, sender, message;
};

extern std::vector<std::string> pos, neg, vocabulary;

//extern list<std::string> tot;

#endif