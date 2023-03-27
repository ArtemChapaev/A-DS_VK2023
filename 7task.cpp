// 7.3. Binary MSD для long long.
// Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел не больше 10E-6.
// Отсортировать массив методом MSD по битам (бинарный QuickSort).


#include <iostream>

int FindMaxBitNum(unsigned long long *arr, int size);

int Partition(unsigned long long *arr, int size, unsigned long long pivotValue);

void BinaryMSDSort(unsigned long long *arr, int size, unsigned long long pivotValue);

int main() {
    int size = 0;
    std::cin >> size;

    unsigned long long *arr = new unsigned long long[size];
    for (int i = 0; i < size; ++i) {
        std::cin >> arr[i];
    }

    int maxBitNum = FindMaxBitNum(arr, size);
    // if maximum value in array = 0, FindMaxBitNum return -1
    if (maxBitNum != -1) {
        unsigned long long pivotValue = 1;
        pivotValue <<= maxBitNum;
        BinaryMSDSort(arr, size, pivotValue);
    }

    for (int i = 0; i < size; ++i) {
        std::cout << arr[i];
        if (i != size - 1) {
            std::cout << " ";
        }
    }

    delete[] arr;
    return 0;
}

void BinaryMSDSort(unsigned long long *arr, int size, unsigned long long pivotValue) {
    if (pivotValue == 0) {
        return;
    }
    // return first elem of 1xxxxx numbers
    int border = Partition(arr, size, pivotValue);
    if (border > 0) {
        BinaryMSDSort(arr, border, pivotValue >> 1);
    }
    if (border < size) {
        BinaryMSDSort(arr + border, size - border, pivotValue >> 1);
    }
}

int FindMaxBitNum(unsigned long long *arr, int size) {
    // search of array's maximum
    unsigned long long m = 0;
    for (int i = 0; i < size; ++i) {
        m = arr[i] > m ? arr[i] : m;
    }

    // calculate bit amount. if maximum value in array = 0, return -1
    int maxBitNum = -1;
    while (m != 0) {
        m >>= 1;
        ++maxBitNum;
    }

    return maxBitNum;
}

int Partition(unsigned long long *arr, int size, unsigned long long pivotValue) {
    if (size <= 1) {
        return 0;
    }
    int i = 0;
    int j = size - 1;
    while (i <= j) {
        // while needed bit of arr[i] don't exist
        while (i < size && !(arr[i] & pivotValue)) {
            ++i;
        }

        // while needed bit of arr[i] exist
        while (j >= 0 && arr[j] & pivotValue) {
            --j;
        }

        if (i <= j) {
            std::swap(arr[i], arr[j]);
        }
    }
    // return first elem of 1xxxxx numbers
    return i;
}
