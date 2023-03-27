// 6.2.
// Реализуйте стратегию выбора опорного элемента “медиана трёх”.
// Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

// Дано множество целых чисел из [0..10^9] размера size.
// Используя алгоритм поиска k-ой порядковой статистики, требуется найти следующие параметры множества:
//  1) 10% перцентиль
//  2) медиана
//  3) 90% перцентиль

// Требования: к дополнительной памяти: O(size).
// Среднее время работы: O(size)
// Должна быть отдельно выделенная функция partition.
// Рекурсия запрещена.
// Решение должно поддерживать передачу функции сравнения снаружи.

// Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.

// Описание для случая прохода от начала массива к концу:
// Выбирается опорный элемент. Опорный элемент меняется с последним элементом массива.
// Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы,
// строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
// Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
// Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
// Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j.
// Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
// В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.


#include <cassert>
#include <iostream>

template<class T, class Compare>
T FindKStat(T *array, int size, int k, Compare cmp);

template<class T, class Compare>
int Partition(T *array, int size, Compare cmp);

template<class T, class Compare>
int FindMedianThree(T *array, int size, Compare cmp);

template<class T>
class IsLessCmpDefault {
public:
    bool operator()(const T &l, const T &r) {
        return l < r;
    }
};

int main() {
    int size = 0;
    std::cin >> size;

    unsigned *array = new unsigned[size];
    for (int i = 0; i < size; ++i) {
        std::cin >> array[i];
    }

    IsLessCmpDefault<unsigned> cmp;
    std::cout << FindKStat(array, size, size * 0.1, cmp) << "\n"
              << FindKStat(array, size, size * 0.5, cmp) << "\n"
              << FindKStat(array, size, size * 0.9, cmp);

    delete[] array;
    return 0;
}

template<class T, class Compare>
T FindKStat(T *array, int size, int k, Compare cmp) {
    assert(k < size);

    int pivotPos = Partition(array, size, cmp);

    // borders show current array's size. firstly the size is equal size of T *array
    int leftBorder = 0;
    int rightBorder = size;

    while (pivotPos != k) {
        if (pivotPos < k) {
            leftBorder = pivotPos + 1;
        } else {
            rightBorder = pivotPos;
        }
        // call Partition with new size of array
        pivotPos = leftBorder + Partition(leftBorder + array, rightBorder - leftBorder, cmp);
    }

    return array[k];
}

template<class T, class Compare>
int Partition(T *arr, int n, Compare cmp) {
    if (n <= 1) {
        return 0;
    }

    // search of pivot's position
    int indexMed = n < 3 ? 0 : FindMedianThree(arr, n, cmp);
    std::swap(arr[indexMed], arr[n - 1]);

    int i = n - 1;
    int j = n - 1;
    while (j != 0) {
        // comparing of the pivot and element that is before j
        if (cmp(arr[n - 1], arr[--j])) {
            std::swap(arr[--i], arr[j]);
        }
        // else we just move j to one
    }
    // set pivot to needed position
    std::swap(arr[i], arr[n - 1]);

    return i;
}

template<class T, class Compare>
int FindMedianThree(T *arr, int n, Compare cmp) {

    // if n/3-th elem is median
    if ((cmp(arr[0], arr[n / 3]) && cmp(arr[n / 3], arr[n * 2 / 3])) ||
        (cmp(arr[n * 2 / 3], arr[n / 3]) && cmp(arr[n / 3], arr[0]))) {
        return n / 3;
    }

    // if first elem is median
    if ((cmp(arr[n / 3], arr[0]) && cmp(arr[0], arr[n * 2 / 3])) ||
        (cmp(arr[n * 2 / 3], arr[0]) && cmp(arr[0], arr[n / 3]))) {
        return 0;
    }

    // if 2n/3-th elem is median
    return n * 2 / 3;
}

