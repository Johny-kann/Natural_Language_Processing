#include "models.h"
#include <vector>

namespace johny
{


std::vector<johny::tweetStyle> parseFile(std::string fileName);

std::vector<std::string> parseFileStrings(std::string fileName);

std::vector<std::string> parseTextToWords(std::string text);

std::vector<std::string> tweetsToWords(std::vector<johny::tweetStyle> tweets, bool pos);

void findWordProb(std::vector<std::string> texts, johny::vocabStatus &word, int vocabLength, bool pos);

void toLower(std::string &text);

void calculateProbOfWords(std::vector<std::string> texts, std::vector<vocabStatus> &vocabStatusList, int vocabLength, bool pos);

}