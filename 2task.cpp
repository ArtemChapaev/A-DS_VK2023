// 2.1.
// Дан отсортированный массив целых чисел A[0..n-1] и массив целых чисел B[0..m-1]. Для каждого элемента массива B[i]
// найдите минимальный индекс k минимального элемента массива A, равного или превосходящего B[i]: A[k] >= B[i]. Если
// такого элемента нет, выведите n. n, m ≤ 10000.

// Требования: Время работы поиска k для каждого элемента B[i]: O(log(k)). Внимание! В этой задаче для каждого
// B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k с помощью экспоненциального
// поиска, а потом уже в нем делать бинарный поиск.

// Формат входных данных.
// В первой строчке записаны числа n и m. Во второй и третьей массивы A и B соответственно.


#include <iostream>
#include <tuple>

std::tuple<unsigned, unsigned> ExponentialSearch(int *arr, unsigned arrLength, int key);

unsigned BinarySearch(int *arr, unsigned arrLength, int key);

void LowerBound(int *A, int n, int *B, int m, int *res);

int main() {
    unsigned n = 0;
    unsigned m = 0;
    std::cin >> n >> m;

    int *A = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    int *B = new int[m];
    for (int i = 0; i < m; ++i) {
        std::cin >> B[i];
    }

    int *res = new int[m];
    LowerBound(A, n, B, m, res);

    for (int i = 0; i < m; ++i) {
        std::cout << res[i];
        if (i != m - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    delete[] A;
    delete[] B;
    delete[] res;
    return 0;
}

void LowerBound(int *A, int n, int *B, int m, int *res) {
    for (int i = 0; i < m; ++i) {

        unsigned leftBorder = 0;
        unsigned rightBorder = 0;
        std::tie(leftBorder, rightBorder) = ExponentialSearch(A, n, B[i]);

        // we use only relevant part
        res[i] = BinarySearch(&A[leftBorder], rightBorder - leftBorder, B[i]) + leftBorder;
    }
}


std::tuple<unsigned, unsigned> ExponentialSearch(int *arr, unsigned arrLength, int key) {
    unsigned leftBorder = 0;
    unsigned rightBorder = 1;
    while (key > arr[rightBorder] && leftBorder != arrLength - 1) { // arrLength-1 -- we check last elem of array
        leftBorder = rightBorder;

        if (rightBorder * 2 < arrLength) {
            rightBorder = rightBorder * 2;
        } else {
            rightBorder = arrLength - 1; // bcs arr[arrLength] is garbage
        }
    }
    return std::make_tuple(leftBorder, rightBorder + 1); // rightBorder+1 points to first elem after relevant part
}

unsigned BinarySearch(int *arr, unsigned arrLength, int key) {
    unsigned firstElem = 0;
    unsigned lastElem = arrLength; // lastElem points to first elem after array
    while (firstElem < lastElem) {
        unsigned middle = (firstElem + lastElem) / 2;

        if (arr[middle] >= key) {
            lastElem = middle; // if key == arr[middle] then firstElem will converge with lastElem
        } else {
            firstElem = middle + 1;
        }
    }
    return firstElem;
}
