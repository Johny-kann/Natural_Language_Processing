#include "models.h"
#include <vector>


std::vector<tweetStyle> parseFile(std::string fileName);

std::vector<std::string> parseFileStrings(std::string fileName);

std::vector<std::string> parseTextToWords(std::string text);

std::vector<std::string> tweetsToWords(std::vector<tweetStyle> tweets, bool pos);