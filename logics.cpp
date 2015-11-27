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

	while (file.good())
	{
		std::getline(file, line);

		std::stringstream lineStream(line);
		std::string                cell;

		tweetStyle tes;
		//	= new tweetStyle;

		std::getline(lineStream, cell, ',');

		(tes).clas = string(cell, 1, cell.length() - 2);

		std::getline(lineStream, cell, ',');
		(tes).date = string(cell, 1, cell.length() - 2);

		std::getline(lineStream, cell, ',');
		(tes).id = string(cell, 1, cell.length() - 2);

		std::getline(lineStream, cell, ',');
		(tes).query = string(cell, 1, cell.length() - 2);

		std::getline(lineStream, cell, ',');
		(tes).message = string(cell, 1, cell.length() - 2);

		tweetVectors.push_back((tes));

	}


	return tweetVectors;
}
