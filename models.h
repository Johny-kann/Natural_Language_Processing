#include <string>
#include <vector>
#include <list>

#ifndef MODELS_H
#define MODELS_H

struct tweetStyle
{
	std::string clas, id, date, query, message;
};

extern std::vector<std::string> pos, neg;

//extern list<std::string> tot;

#endif