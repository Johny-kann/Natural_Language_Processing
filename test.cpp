#include <string>
#include <stdio.h>
#include <vector>

struct cudaString2
{
	char *str;
	int length;
};

struct myString
{
	char *str;
	int *index;
	int totIndexes;
};

void separate(char *c, int length)
{
	char prev, prev2, curr;
	prev2 = c[0];
	prev = c[1];

	for (int i = 2; i < length; i++)
	{
		curr = c[i];

		if (curr == '\"' && prev == ',' && prev == '\"')
		{

		}
	}
}

void tester2(cudaString2 &str)
{
	//	cudaString str;
	std::string string = "Hello";
	str.str = new char[string.size() + 1];

	memcpy(str.str, string.data(), string.size()+1);
	//	str.str = string.data();
	str.length = string.size();
	//printf(str.str);
}

void print2(cudaString2 str)
{
	printf(str.str);
}

int main()
{
	cudaString2 string;

//	tester2(string);
//	print2(string);
//	printf("%s", string.str);

/*	std::vector<double> vec;

	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);
	vec.push_back(6);
	*/

/*	std::vector<std::string> vec;

	std::string str = "hello";

	vec.push_back(str);

	str = "hello2";

	vec.push_back(str);

//	char *a;
//	a = &vec[0][0];

	cudaString2 *cudaStr = new cudaString2[vec.size()];

	cudaStr[0].str = &vec[0][0];
//	cudaStr->str= &vec[0][0];
	cudaStr[1].str[4] = 'j';

//	a[4] = 22;
//	a[2] = '3';

//	printf("%c", *a);
	printf("\n%s  %s", vec[0].c_str(),vec[1].c_str());
	*/

	char *a = new char[7];
	a[0] = '1';
	a[1] = '2';
	a[2] = '3';
	a[3] = '\0';
	a[4] = '4';
	a[5] = '5';
	a[6] = '6';
	a[7] = '\0';

	std::string str(a),str2;
//	str.insert(str.end(), a[0], 3);// (a);
	str2.assign(a+4);

	std::string temp;
	temp = "hello";

	cudaString2 b;
	b.str = &temp[0];
	b.length = temp.size();
	b.str[b.length] = '\0';

//	for (int i = 0; i < b.length; i++)
	//{
		printf("%s %s", b.str,temp.c_str());
//	}

//	printf("%s %s %c", str.c_str(),str2.c_str(),*(b+5));

	getchar();
}