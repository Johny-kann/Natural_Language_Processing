#include "cuda_runtime.h"
#include "device_launch_parameters.h"

struct cudaString
{
	char *str;
	int length;
};

struct myString
{
	char *str;
	short index[10];
	short totIndexes;
};


cudaError_t separateStub(cudaString *linesCuda, myString *tweetsCuda, unsigned int size);

int separator(cudaString *linesCuda, myString *tweetsCuda, unsigned int size);