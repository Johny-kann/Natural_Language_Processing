#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "logics.h"
#include "reductionCuda.cuh"


void johny::toLower(std::string &text)
{
	for (int i = 0; i < text.size(); i++)
	{
		if (text.at(i) >= 'A' && text.at(i) <= 'Z')
			text.at(i) = tolower(text.at(i));
	}


}

void johny::calculateProbOfWords(std::vector<std::string> texts, std::vector<johny::vocabStatus> &vocabStatusList, int vocabLength, bool pos)
{
	for (int i = 0; i < vocabulary.size(); i++)
	{
		johny::findWordProb(texts, vocabStatusList.at(i), vocabulary.size(), pos);

	}
}

void johny::findWordProb(std::vector<std::string> texts, johny::vocabStatus &word, int vocabLength, bool pos)
{
	using namespace std;

//	johny::vocabStatus status;
//	status.word = word;

	int countMatch = 0;
	
	for (string str : texts)
	{
		if (str.compare(word.word) == 0)
			countMatch++;
	}

//	cout << "Matches "<<countMatch<<std::endl;

	if (pos)
		word.posProb = (double)(countMatch + 1)/(texts.size() + vocabLength);
	else if (!pos)
		word.negProb = (double)(countMatch + 1) / (texts.size() + vocabLength);

//	return status;
}

std::vector<std::string> johny::parseTextToWords(std::string text)
{
	using namespace std;

	string word;

	stringstream lineStream(text);

	vector<string> words;

	while (//file.good()
		getline(lineStream, word,' '))
	{
	
		
		words.push_back(word);
		
	}

	return words;

}

std::vector<johny::tweetStyle> johny::parseFileTarget(std::string fileName)
{
	using namespace std;
	using namespace johny;

	ifstream file(fileName); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	string value;

	vector<tweetStyle> tweetVectors;
	
	string line;

	while (std::getline(file, line))
	{
		

		std::stringstream lineStream(line);
		std::string                cell;

		tweetStyle tes;
		
		int cc = 0;

		while (std::getline(lineStream, cell, ','))
		{
			switch (cc)
			{
			case 0: (tes).clas = "-1";
			/*	if ((tes).clas.compare("0") == 0)
					johny::status.negWords++;
				else if ((tes).clas.compare("4") == 0)
					johny::status.posWords++;
					*/
				break;

			case 4: (tes).sender = string(cell, 1, cell.length() - 2);
				break;

			case 1: (tes).id = string(cell, 1, cell.length() - 2);
				break;

			case 2: (tes).date = string(cell, 1, cell.length() - 2);
				break;

			case 3: (tes).query = string(cell, 1, cell.length() - 2);
				break;

			case 5: (tes).message = string(cell, 1, cell.length() - 2);
				toLower(tes.message);
				break;

			default: break;
			}

			cc++;
			if (cc == 6)
				cc = 0;

		}



		tweetVectors.push_back((tes));

	}

	file.close();
	return tweetVectors;


}

std::vector<johny::tweetStyle> johny::parseFile(std::string fileName)
{
	using namespace std;
	using namespace johny;

	ifstream file(fileName); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	string value;

	vector<tweetStyle> tweetVectors;
	//	std::vector<std::string>   result;
	string line;

	while (//file.good()
		std::getline(file, line))
	{
//		std::getline(file, line);

//		line[0]

		std::stringstream lineStream(line);
		std::string                cell;

		tweetStyle tes;
		//	= new tweetStyle;
		int cc = 0;

		while(std::getline(lineStream, cell, ','))
		{
			switch (cc)
			{
			case 0: (tes).clas = string(cell, 1, cell.length() - 2);
				if ((tes).clas.compare("0") == 0)
					johny::status.negWords++;
				else if ((tes).clas.compare("4") == 0)
					johny::status.posWords++;
				break;

			case 4: (tes).sender = string(cell, 1, cell.length() - 2);
				break;

			case 1: (tes).id = string(cell, 1, cell.length() - 2);
				break;

			case 2: (tes).date = string(cell, 1, cell.length() - 2);
				break;

			case 3: (tes).query = string(cell, 1, cell.length() - 2);
				break;

			case 5: (tes).message = string(cell, 1, cell.length() - 2);
				toLower(tes.message);
				break;

			default: break;
			}
		
			cc++;
			if (cc == 6)
				cc = 0;
	
		}

	

		tweetVectors.push_back((tes));

	}

	file.close();
	return tweetVectors;
}

std::vector<johny::tweetStyle> johnyGPU::parseFileCuda(std::string fileName)
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
		for (int j = 0; j < lines[i].size() + 1; j++)
		{
			if (j == lines[i].size())
				charLines[i * 300 + j] = '\0';
			else
				charLines[i * 300 + j] = lines[i][j];

		}

	}

	//	charSeparator(charLines, indexes, lines.size());
	if (separator(charLines, indexes, lines.size()) != 0)
	{
		getchar();
		exit(-3);
	}
	else
	{
		cout << "Separator executed";
	}

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

	file.close();
	return tweetVectors;
}

std::vector<std::string> johny::tweetsToWords(std::vector<johny::tweetStyle> tweets, bool pos)
{
	using namespace std;

	short flag;

	vector<string> text_words;
	

	for (tweetStyle tweet : tweets)
	{
		if (tweet.clas.compare("0")==0)  // 0 negative, 2 neutral , 4 positive
		{
			flag = 1;
		//	cout <<"\\nnr "<< tweet.clas<<" nnr/";
		}
		else if (tweet.clas.compare("4")==0)
		{
			flag = -1;
		//	cout <<"\\ppr "<< tweet.clas<<" ppr/";
		}
		else
		{
			flag = 0;
		//	cout << "\\ttr "<<tweet.clas<<" ttr/";
		}

		if (pos && flag == -1)
		{
			    vector<string> words = parseTextToWords(tweet.message);
				text_words.insert(end(text_words), begin(words), end(words));
		}
		else if (!pos && flag == 1)
		{
			vector<string> words = parseTextToWords(tweet.message);
			text_words.insert(end(text_words), begin(words), end(words));
		}
			
	}

	return text_words;
}


bool johny::wordToVocabsMatch(std::string word, std::vector<vocabStatus> vocabStatusList, bool pos, double &prob)
{
	for (int i = 0; i < vocabStatusList.size(); i++)
	{
		if (word.compare(vocabStatusList.at(i).word) == 0)
		{
			if (pos)
				prob = vocabStatusList.at(i).posProb;
			else if (!pos)
				prob = vocabStatusList.at(i).negProb;

			return true;
		}
	}

	prob = constProb;
	return false;
}


double johny::showtweetProb(std::vector<std::string> texts, std::vector<vocabStatus> vocabStatusList, bool pos)
{
	double probV;
	if (pos)
		probV = johny::status.posProb;
	else if (!pos)
		probV = johny::status.negProb;

	double prob = probV;

	for (int i = 0; i < texts.size(); i++)
	{
		double temp;
		wordToVocabsMatch(texts.at(i), vocabStatusList, pos, temp);
		prob *= temp;
		
	}

	return prob;
}

std::vector<std::string> johny::parseFileStrings(std::string fileName)
{
	using namespace std;

	ifstream file(fileName);

	string line;

	vector<string> words;

	while (//file.good()
		getline(file, line))
	{
		//		std::getline(file, line);

	//	std::stringstream lineStream(line);
	//	std::string cell;

		toLower(line);

		words.push_back(line);
	}

	file.close();

	return words;
}