#include <iostream>
#include <tuple>

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

int main() {
    unsigned n = 0;
    unsigned m = 0;
    std::cin >> n >> m;

    int A[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    // for better speed we don't create array and use std::cin in loop with functions
    for (int i = 0; i < m; ++i) {
        int element = 0;
        std::cin >> element;

        unsigned leftBorder = 0;
        unsigned rightBorder = 0;
        std::tie(leftBorder, rightBorder) = ExponentialSearch(A, n, element);

        // we use only relevant part
        std::cout << BinarySearch(&A[leftBorder], rightBorder - leftBorder, element) + leftBorder;
        if (i != m - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}