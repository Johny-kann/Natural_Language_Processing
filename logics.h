#include "models.h"
#include <vector>

namespace johny
{

#define constProb 0.000001

std::vector<johny::tweetStyle> parseFile(std::string fileName);

std::vector<johny::tweetStyle> parseFileTarget(std::string fileName);

std::vector<std::string> parseFileStrings(std::string fileName);

std::vector<std::string> parseTextToWords(std::string text);

void wordHandler(char *c, int *indexes, int lineNum);

std::vector<std::string> tweetsToWords(std::vector<johny::tweetStyle> tweets, bool pos);

void tweetsToWords(std::vector<johny::tweetStyle> tweets, std::vector<std::string> &posWords, std::vector<std::string> &negWords);



void findWordProb(std::vector<std::string> texts, johny::vocabStatus &word, int vocabLength, bool pos);

void toLower(std::string &text);

void calculateProbOfWords(std::vector<std::string> texts, std::vector<vocabStatus> &vocabStatusList, int vocabLength, bool pos);

double showtweetProb(std::vector<std::string> texts, std::vector<vocabStatus> vocabStatusList, bool pos);

bool wordToVocabsMatch(std::string word, std::vector<vocabStatus> vocabStatusList, bool pos, double &prob);

}

namespace johnyGPU
{
	std::vector<johny::tweetStyle> parseFileCuda(std::string fileName, bool source);

	void tweetsToWordsGPU(std::vector<johny::tweetStyle> tweets, std::vector<std::string> &posWords, std::vector<std::string> &negWords);
}