#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "logics.h"


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

			case 1: (tes).id = string(cell, 1, cell.length() - 2);
				break;

			case 2: (tes).date = string(cell, 1, cell.length() - 2);
				break;

			case 3: (tes).query = string(cell, 1, cell.length() - 2);
				break;

			case 4: (tes).message = string(cell, 1, cell.length() - 2);
				break;

			default: break;
			}
		
			cc++;
			if (cc == 5)
				cc = 0;
	
		}

	

		tweetVectors.push_back((tes));

	}

	file.close();
	return tweetVectors;
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

	
	/*	int cc = 0;

		while (std::getline(lineStream, cell, ','))
		{
			switch (cc)
			{
			case 0: (tes).clas = string(cell, 1, cell.length() - 2);
				break;

			case 1: (tes).id = string(cell, 1, cell.length() - 2);
				break;

			case 2: (tes).date = string(cell, 1, cell.length() - 2);
				break;

			case 3: (tes).query = string(cell, 1, cell.length() - 2);
				break;

			case 4: (tes).message = string(cell, 1, cell.length() - 2);
				break;

			default: break;
			}

			cc++;
			if (cc == 5)
				cc = 0;

		}



		tweetVectors.push_back((tes));
		*/

	}

	file.close();

	return words;
}