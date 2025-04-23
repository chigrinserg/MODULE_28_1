#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <random>
#include <ctime>

bool flag = false;
void merge(int* arr, int l, int m, int r)
{
	int nl = m - l + 1;
	int nr = r - m;

	int* left = new int[nl];
	int* right = new int[nr];

	for (int i = 0; i < nl; i++)
		left[i] = arr[l + i];
	for (int j = 0; j < nr; j++)
		right[j] = arr[m + j + 1];

	int i = 0, j = 0;
	int k = l;

	while (i < nl && j < nr)
	{
		if (left[i] < right[j])
		{
			arr[k++] = left[i++];
		}
		else
		{
			arr[k++] = right[j++];
		}
	}

	while (i < nl)
		arr[k++] = left[i++];

	while (j < nr)
		arr[k++] = right[j++];

	delete[] left;
	delete[] right;
}
void mergeSort(int* arr, int l, int r)
{
	if (l >= r)
		return;

	int m = (l + r - 1) / 2;
	if (flag && (m - l > 100000))
	{
		std::future<void> f = std::async(std::launch::async, [&]() { mergeSort(arr, l, m); });
		f.get();
		mergeSort(arr, m + 1, r);
	}
	else
	{
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);
	}
	merge(arr, l, m, r);
}

int main()
{
	setlocale(LC_ALL, "Rus");
	srand(0);
	int arr_size = 9000000;
	int* arr = new int[arr_size];
	std::cout << "Создание исходного массива " << std::endl;
	for (size_t i = 0; i < arr_size; i++)
	{
		arr[i] = rand() % 50000;
	}

	std::cout << "Запуск однопоточной сортировки..." << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	mergeSort(arr, 0, arr_size - 1);
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Время выполнения сортировки: " << elapsed.count() << " сек." << std::endl;

	for (long i = 0; i < arr_size - 1; i++) //Проверка состояния массива
	{
		if (arr[i] > arr[i + 1]) 
		{
			std::cout << "Массив не отсортирован!" << std::endl;
			break;
		}
	}

	std::cout << "\nСоздание второго исходного массива: " << std::endl;
	flag = true;
	for (size_t i = 0; i < arr_size; i++)
	{
		arr[i] = rand() % 5000;
	}
	std::cout << "Запуск многопоточной сортировки: " << std::endl;
	auto _start = std::chrono::high_resolution_clock::now();
	mergeSort(arr, 0, arr_size - 1);
	auto _finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> _elapsed = _finish - _start;
	std::cout << "Время выполнения сортировки: " << _elapsed.count() << " сек." << std::endl;

	for (long i = 0; i < arr_size - 1; i++) // Проверка состояния массива:
	{
		if (arr[i] > arr[i + 1]) {
			std::cout << "Массив не отсортирован! " << std::endl;
			break;
		}
	}
	delete[] arr;
	return 0;
}
