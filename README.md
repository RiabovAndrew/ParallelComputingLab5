# ParallelComputingLab5
 
## Отчёт по лабораторной работе №5   

по дисциплине "Параллельные вычисления"   
студента группы ПА-18-2   
Рябова Андрея Дмитриевича   

### Постановка задачи:   

Инструкции:

![image](https://user-images.githubusercontent.com/43186510/117068446-f6c56100-ad33-11eb-801d-5ad58b182589.png)

### Выполнение:   

#### 1. Реализация функций для работы с массивом

![image](https://user-images.githubusercontent.com/43186510/117068570-1fe5f180-ad34-11eb-85c5-6dad53144821.png)

#### 2. Объявление переменных, используемых в потоках

![image](https://user-images.githubusercontent.com/43186510/117068738-57ed3480-ad34-11eb-9c1a-84437dc84df4.png)

#### 3. Инициализация вектора на 0 потоке

![image](https://user-images.githubusercontent.com/43186510/117068840-705d4f00-ad34-11eb-93b4-c5e7cb8474a2.png)

#### 4. Разсылка на все потоки

![image](https://user-images.githubusercontent.com/43186510/117068890-80752e80-ad34-11eb-85ce-ccde09d892f2.png)

#### 5. Работа над личными данными на каждом потоке

![image](https://user-images.githubusercontent.com/43186510/117068981-9b47a300-ad34-11eb-8428-64fcdce4c442.png)

#### 6. Сбор информации

![image](https://user-images.githubusercontent.com/43186510/117069039-af8ba000-ad34-11eb-8a01-0fe5e2a3ccf9.png)

#### 7. Вывод результатов

![image](https://user-images.githubusercontent.com/43186510/117069096-c3370680-ad34-11eb-9cfa-eee135b0d490.png)

#### 8. Очистка данных и завершение работы MPI

![image](https://user-images.githubusercontent.com/43186510/117069207-e792e300-ad34-11eb-8eb4-05367c1e32b2.png)

### Код

```
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

```

