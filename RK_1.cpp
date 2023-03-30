#include <iostream>

unsigned MinBinarySearch(int *arr, unsigned arrLength);

int main() {
    unsigned n = 0;
    std::cin >> n;

    int *arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    std::cout << MinBinarySearch(arr, n);
    delete[] arr;
    return 0;
}

unsigned MinBinarySearch(int *arr, unsigned arrLength) {
    unsigned firstElem = 0;
    unsigned lastElem = arrLength - 1;
    while (firstElem < lastElem) {
        unsigned middle = (firstElem + lastElem) / 2;

        if (arr[middle] < arr[lastElem]) {
            lastElem = middle;
        } else {
            firstElem = middle + 1;
        }
    }
    return firstElem;
}
