
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




__global__ void addKernel(int *c, const int *a, const int *b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}

__global__ void testKernel(cudaString str)
{
	int i = blockIdx.x*blockDim.x + threadIdx.x;
//	str.at(0);
//	std::cout << str;
	printf("%s\t",str.str);
}


int main4()
{
	

/*	cudaError_t cudaStatus = testWithCuda(10);

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addWithCuda failed!");
		return 1;
	}

	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}*/
	


	getchar();

	return 0;
}

int mainK()
{
    const int arraySize = 5;
    const int a[arraySize] = { 1, 2, 3, 4, 5 };
    const int b[arraySize] = { 10, 20, 30, 40, 50 };
    int c[arraySize] = { 0 };

    // Add vectors in parallel.
    cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }

    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
        c[0], c[1], c[2], c[3], c[4]);

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}

cudaError_t separateStub(char *chars, int *indexes, unsigned int size);

cudaError_t tweetToWordStub(char *chars, int *indexes, unsigned int size);

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

cudaError_t testWithCuda(unsigned int size)
{
/*	std::string str = "Hello";

	cudaString string;

	string.str = new char[str.size() + 1];

	memcpy(string.str, str.c_str(), str.size());
	*/

	char *strr = "Hello";
	cudaString str;
	str.str = strr;

	cudaError_t cudaStatus;

	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}

	

	// Launch a kernel on the GPU with one thread for each element.
	testKernel << <1, size >> >(str);

	// Check for any errors launching the kernel
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
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

Error:

	return cudaStatus;
}


// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size)
{
    int *dev_a = 0;
    int *dev_b = 0;
    int *dev_c = 0;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    addKernel<<<1, size>>>(dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
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
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);
    
    return cudaStatus;
}
