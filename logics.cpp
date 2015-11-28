#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "logics.h"


std::vector<std::string> parseTextToWords(std::string text)
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

std::vector<tweetStyle> parseFile(std::string fileName)
{
	using namespace std;

	ifstream file(fileName); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
	string value;

	vector<tweetStyle> tweetVectors;
	//	std::vector<std::string>   result;
	string line;

	while (//file.good()
		std::getline(file, line))
	{
//		std::getline(file, line);

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


std::vector<std::string> tweetsToWords(std::vector<tweetStyle> tweets, bool pos)
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

std::vector<std::string> parseFileStrings(std::string fileName)
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
		std::string cell;

		words.push_back(line);

	
	

	}

	file.close();

	return words;
}