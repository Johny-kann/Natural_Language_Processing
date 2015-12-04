
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "reductionCuda.cuh"

#include <stdio.h>
#include <string>
#include <iostream>

/*struct cudaString
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
*/

cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);
cudaError_t testWithCuda(unsigned int size);

__device__ void wordHandlerDevice(char *c, int *indexes, int lineNum)
{
	char prev = '0', curr;
	indexes[0] = 150 * lineNum;

	int count = 0;

	for (int i = 0; c[i] != '\0'; i++)
	{
		curr = c[i];
		if (i != 0)
			prev = c[i - 1];

		if (!((curr >= 'A' && curr <= 'Z') || (curr >= 'a' && curr <= 'z') || (curr >= '0' && curr <= '9')))
		{
			c[i] = '\0';
			curr = '\0';
		}

		if (prev == '\0' && curr != '\0')
		{
			count++;
			indexes[count] = 150 * lineNum + i;
		}
	}

	count++;
	indexes[count] = -1;
}


__device__ __host__ int stringCompare(char *source, char *dest)
{
	int i;
	for (i = 0; source[i] != '\0'; i++)
	{
		if (dest[i] > source[i])
			return 1;
		else if (dest[i] < source[i])
			return -1;
	}

	if (dest[i] != '\0')
		return 1;

	return 0;
}

__device__ void compareWords(char *tweet, int *posNeg, char *posWords, unsigned int posWordSize, char *negWords, unsigned int negWordSize)
{
	int indexes[50];
	char prev = '0', curr;
	indexes[0] = 0;

	int count = 0;

	for (int i = 0; tweet[i] != '\0'; i++)
	{
		curr = tweet[i];
		if (i != 0)
			prev = tweet[i - 1];

		if (!((curr >= 'A' && curr <= 'Z') || (curr >= 'a' && curr <= 'z') || (curr >= '0' && curr <= '9')))
		{
			tweet[i] = '\0';
			curr = '\0';
		}

		if (prev == '\0' && curr != '\0')
		{
			count++;
			indexes[count] = i;
		}
	}

	count++;
	indexes[count] = -1;

	int prob = 0;

	for (int i = 0; indexes[i] != -1; i++)
	{
		for (int j = 0; j < posWordSize; j++)
		{
			if (stringCompare(&tweet[indexes[i]], &posWords[25*j]) == 0)
			{
			//	printf("\nPositive Prob for word %s in thread %d", &tweet[indexes[i]], threadIdx.x);
				prob++; break;
			}
			
		}

		for (int j = 0; j < negWordSize; j++)
		{
			if (stringCompare(&tweet[indexes[i]], &negWords[25*j]) == 0)
			{
				prob--; break;
			}
		}
	}

	if (prob > 0)
	{
		*posNeg = 1;
	}
	else if (prob < 0)
	{
		*posNeg = -1;
	}
	else
	{
		*posNeg = 0;
	}
}

__device__ void charLineHandlerDevice(char *c, int *indexes, unsigned int lineNum)
{
	char prev, prev2, curr;
//	indexes[0] = 300 * lineNum;

	int count = 0;

//	printf("kernal %d", blockIdx.x*blockDim.x+threadIdx.x);

	int i;
	for (i = 2; c[i] != '\0'; i++)
	{
		//	c[i] = c[i] + 1;
		//	printf("%c", c[i]);
		curr = c[i];
		prev = c[i - 1];
		prev2 = c[i - 2];

		if (curr == '\"' && prev == ',' && prev2 == '\"')
		{
			c[i] = '\0';
			c[i - 1] = '\0';
			c[i - 2] = '\0';
		}
		else if (prev == '\0')
		{
			count++;
			indexes[count] = 300 * lineNum + i;
		}

		if (curr >= 'A' && curr <= 'Z')
			c[i]=c[i]+32;

	}

		c[0] = '\0';
		c[i-1] = '\0';
		indexes[0] = 300 * lineNum + 1;
		count++;
		indexes[count] = -1;
}

__global__ void tweetToWordKernal(char *message, int *index, unsigned int size)
{
	int i = blockDim.x*blockIdx.x + threadIdx.x;

	if (i<size)
		wordHandlerDevice(&message[150 * i], &index[70 * i], i);
	
}

__global__ void tweetToProbKernal(char *tweets, int *prob, unsigned int size,char *posWords, unsigned int posWordSize, char *negWords, unsigned int negWordSize)
{
	int i = blockDim.x*blockIdx.x + threadIdx.x;

	if (i < size)
	{	
		compareWords(&tweets[150 * i], &prob[i], posWords, posWordSize, negWords, negWordSize);

	}

}

__global__ void separateKernal(char *charIn, int *indexes, unsigned int size)
{
	int i = blockDim.x*blockIdx.x + threadIdx.x;

//	__syncthreads();
	if (i < size)
	{
		charLineHandlerDevice(charIn + i * 300, indexes + i * 20, i);

	//	printf("%d ", i);
	}
//	__syncthreads();


}



cudaError_t separateStub(char *chars, int *indexes, unsigned int size);

cudaError_t tweetToWordStub(char *chars, int *indexes, unsigned int size);

cudaError_t tweetToProbStub(char *tweets, int *prob, int size, char *posWords, int posWordSize, char *negWords, int negWordSize);

int tweetToProb(char *tweets, int *prob, int size, char *posWords, int posWordSize, char *negWords, int negWordSize)
{
	cudaError_t cudaStatus = tweetToProbStub(tweets, prob, size, posWords, posWordSize, negWords, negWordSize);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addWithCuda failed!");
		return 1;
	}

	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}

	return 0;
}

int tweetToWordCuda(char *message, int *index, unsigned int size)
{
	// Add vectors in parallel.
	cudaError_t cudaStatus = tweetToWordStub(message, index, size);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addWithCuda failed!");
		return 1;
	}

	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}

	return 0;
}

int separator(char *chars, int *indexes, unsigned int size)
{
	// Add vectors in parallel.
	cudaError_t cudaStatus = separateStub(chars, indexes, size);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addWithCuda failed!");
		return 1;
	}

	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}

	return 0;
}

cudaError_t tweetToProbStub(char *tweets, int *prob, int size, char *posWords, int posWordSize, char *negWords, int negWordSize)
{
	char *dev_tweet_in;
	int *dev_prob_out;

	char *dev_posWords_in;
	char *dev_negWords_in;

	cudaError_t cudaStatus;

	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_tweet_in, 150 * size * sizeof(char));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_prob_out, size * sizeof(int));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_posWords_in, 25 * posWordSize * sizeof(char));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	cudaStatus = cudaMalloc((void**)&dev_negWords_in, 25 * negWordSize * sizeof(char));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}


	// Copy input vectors from host memory to GPU buffers.
	cudaStatus = cudaMemcpy(dev_tweet_in, tweets, 150 * size * sizeof(char), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cuda String cudaMemcpy failed!");
		goto Error;
	}

//	for (int i = 0; i < 3; i++)
//		printf("%s ", &posWords[i * 25]);

	cudaStatus = cudaMemcpy(dev_posWords_in, posWords, 25 * posWordSize * sizeof(char), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cuda String cudaMemcpy failed!");
		goto Error;
	}

	cudaStatus = cudaMemcpy(dev_negWords_in, negWords, 25 * negWordSize * sizeof(char), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cuda String cudaMemcpy failed!");
		goto Error;
	}

	/*---------------------------Kernal----------------------*/
	dim3 grid;
	dim3 block;

	block.x = 1024;
	block.y = 1;
	block.z = 1;

	grid.x = ceil((double)size / block.x);
	grid.y = 1;
	grid.z = 1;

	
	tweetToProbKernal << < grid, block >> >(dev_tweet_in , dev_prob_out , size, dev_posWords_in , posWordSize , dev_negWords_in , negWordSize);

	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "tweetToProbKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching tweetToProbKernel!\n", cudaStatus);
		goto Error;
	}

	// Copy output vector from GPU buffer to host memory.
	cudaStatus = cudaMemcpy(prob, dev_prob_out ,size * sizeof(int), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "Prob cudaMemcpy failed!");
		goto Error;
	}


Error:
	cudaFree(dev_negWords_in);
	cudaFree(dev_posWords_in);
	cudaFree(dev_prob_out);
	cudaFree(dev_tweet_in);
//	cudaFree(dev_);

	return cudaStatus;

}

cudaError_t tweetToWordStub(char *chars, int *indexes, unsigned int size)
{
	char *dev_in;
	int *dev_out;
	cudaError_t cudaStatus;

	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}

	// Allocate GPU buffers for three vectors (two input, one output).

//	std::cout << 150 * size*sizeof(char) << '\t';
	cudaStatus = cudaMalloc((void**)&dev_in, 150 * size * sizeof(char));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	// Allocate GPU buffers for three vectors (two input, one output)    .

//	std::cout << 70 * size*sizeof(int) << '\t';
	cudaStatus = cudaMalloc((void**)&dev_out, 70 * size * sizeof(int));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}


	// Copy input vectors from host memory to GPU buffers.
	cudaStatus = cudaMemcpy(dev_in, chars, 150 * size * sizeof(char), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cuda String cudaMemcpy failed!");
		goto Error;
	}

	dim3 grid;
	dim3 block;

	block.x = 1024;
	block.y = 1;
	block.z = 1;
	grid.x = ceil((double)size / 1024);
	grid.y = 1;
	grid.z = 1;

	std::cout << grid.x << block.x;

	tweetToWordKernal << < grid, block >> >(dev_in, dev_out, size);
	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "separateKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		goto Error;
	}

	// Copy output vector from GPU buffer to host memory.
	cudaStatus = cudaMemcpy(indexes, dev_out, 70 * size * sizeof(int), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "Indexes Cuda tweet to word cudaMemcpy failed!");
		goto Error;
	}

	cudaStatus = cudaMemcpy(chars, dev_in, 150 * size * sizeof(char), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "chars Cuda tweet to word cudaMemcpy failed!");
		goto Error;
	}

	
Error:
	cudaFree(dev_in);
	cudaFree(dev_out);

	return cudaStatus;
}

cudaError_t separateStub(char *chars, int *indexes, unsigned int size)
{
	char *dev_in;
	int *dev_out;
	cudaError_t cudaStatus;

	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}

	// Allocate GPU buffers for three vectors (two input, one output).

	std::cout << 300 * size*sizeof(char)<<'\t';
	cudaStatus = cudaMalloc((void**)&dev_in, 300*size * sizeof(char));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}

	// Allocate GPU buffers for three vectors (two input, one output)    .

	std::cout << 20 * size*sizeof(int) << '\t';
	cudaStatus = cudaMalloc((void**)&dev_out, 20*size * sizeof(int));
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
		goto Error;
	}


	// Copy input vectors from host memory to GPU buffers.
	cudaStatus = cudaMemcpy(dev_in, chars, 300*size * sizeof(char), cudaMemcpyHostToDevice);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cuda String cudaMemcpy failed!");
		goto Error;
	}

	dim3 grid;
	dim3 block;

	block.x = 512;
	block.y = 1;
	block.z = 1;
	grid.x = ceil((double)size / 512);
	grid.y = 1;
	grid.z = 1;

	std::cout << grid.x << block.x;

		separateKernal << < grid, block >> >(dev_in,dev_out,size);
	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "separateKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
		goto Error;
	}

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	cudaStatus = cudaDeviceSynchronize();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
		goto Error;
	}

	// Copy output vector from GPU buffer to host memory.
	cudaStatus = cudaMemcpy(indexes, dev_out, 20 * size * sizeof(int), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "tweets Cuda cudaMemcpy failed!");
		goto Error;
	}

	cudaStatus = cudaMemcpy(chars, dev_in, 300 * size * sizeof(char), cudaMemcpyDeviceToHost);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "tweets Cuda cudaMemcpy failed!");
		goto Error;
	}

//	for (int i = 0; i < 300*size; i++)
//		std::cout << chars[i];

//	for (int i = 0; i < 20 * size; i++)
//		std::cout << indexes[i]<<' ';
Error:
	cudaFree(dev_in);
	cudaFree(dev_out);

	return cudaStatus;
}


