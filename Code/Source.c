#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mpi.h"

void init_vector_rand(int* vector, const int size)
{
	for (int i = 0; i < size; i++)
	{
		vector[i] = rand() % 10;
	}
}

void print_vector(int* vector, const int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("%i ", vector[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	int task_quantity, rank;

	MPI_Status status;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &task_quantity);

	printf("Hello MPI From process = %d, total number of processes: %d\n", rank, task_quantity);

	const int arr_size = task_quantity;
	int* arr = (int*)malloc(arr_size * sizeof(int));
	int* arr_result = (int*)malloc(arr_size * sizeof(int));
	int r;

	if (rank == 0)
	{
		init_vector_rand(arr, arr_size);
	}

	MPI_Bcast(arr, arr_size, MPI_INT, 0, MPI_COMM_WORLD);

	int a = arr[rank];
	
	//MPI_Reduce(&a, &r, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&a, &r, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	//MPI_Reduce(&a, &r, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	//MPI_Reduce(&a, &r, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);

	MPI_Gather(&a, 1, MPI_INT, arr_result, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		printf("res = %i\n", r);
		print_vector(arr_result, arr_size);
	}

	free(arr);
	free(arr_result);
	MPI_Finalize();
}
