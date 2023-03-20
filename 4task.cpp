// 4.1 Слияние массивов.
// Напишите программу, которая использует кучу для слияния K отсортированных массивов суммарной длиной N.
// Требования: время работы O(N * logK). Ограничение на размер кучи O(K).

// Решение всех задач данного раздела предполагает использование кучи, реализованной в виде шаблонного класса.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Куча должна быть динамической.

// Формат входных данных.
// Сначала вводится количество массивов K. Затем по очереди размер каждого массива и элементы массива.
// Каждый массив упорядочен по возрастанию.

// Формат выходных данных.
// Итоговый отсортированный массив.


#include <iostream>
#include <cassert>

const int INITIAL_SIZE = 4;

void MergeArrays(int **arrs, int nArray, int *arrSize, int *sortedArr, int nElement);

template<class T, class Compare>
class Heap {
public:
    Heap(Compare cmp) : buffer(nullptr), bufferSize(0), size(0), cmp(cmp) {}

    Heap(const T *arr, size_t size, Compare cmp);

    ~Heap();

    Heap(Heap &) = delete;
    Heap &operator=(const Heap &h) = delete;

    void Add(const T &el);
    T ExtractMax();
    const T &Peek() const;
    size_t Size() const;

private:
    void grow();

    void SiftDown(size_t i);
    void SiftUp(size_t i);

    T *buffer;
    size_t bufferSize;
    size_t size;
    Compare cmp;
};

struct Elem {
    int value;
    int valueIndex;
    int arrayIndex;
};

// large because we need min-heap
class IsLargeCmpElem {
public:
    bool operator()(const Elem &l, const Elem &r) {
        return l.value > r.value;
    }
};

int main() {
    unsigned nArray = 0;
    std::cin >> nArray;

    int **arrs = new int *[nArray];
    int *arrSize = new int[nArray];

    for (int i = 0; i < nArray; ++i) {
        std::cin >> arrSize[i];
        arrs[i] = new int[arrSize[i]];
        for (int j = 0; j < arrSize[i]; ++j) {
            std::cin >> arrs[i][j];
        }
    }

    int nElement = 0;
    for (int i = 0; i < nArray; ++i) {
        nElement += arrSize[i];
    }

    int *sortedArr = new int[nElement];
    MergeArrays(arrs, nArray, arrSize, sortedArr, nElement);

    for (int i = 0; i < nElement; ++i) {
        std::cout << sortedArr[i];
        if (i != nElement - 1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    delete[] sortedArr;
    delete[] arrSize;
    for (int i = 0; i < nArray; ++i) {
        delete[] arrs[i];
    }
    delete[] arrs;
    return 0;
}

template<class T, class Compare>
Heap<T, Compare>::Heap(const T *arr, size_t size, Compare cmp) : buffer(nullptr), bufferSize(0), size(0), cmp(cmp) {
    for (const T &el: arr) {
        Add(el);
    }
}

template<class T, class Compare>
Heap<T, Compare>::~Heap<T, Compare>() {
    delete[] buffer;
}

template<class T, class Compare>
void Heap<T, Compare>::grow() {
    bufferSize = bufferSize == 0 ? INITIAL_SIZE : bufferSize * 2;
    T *oldBuffer = buffer;

    buffer = new T[bufferSize];
    for (int i = 0; i < size; ++i) {
        // without sift because oldBuffer was heap
        buffer[i] = oldBuffer[i];
    }

    if (oldBuffer != nullptr) {
        delete[] oldBuffer;
    }
}

template<class T, class Compare>
void Heap<T, Compare>::Add(const T &el) {
    if (bufferSize == size) {
        grow();
    }

    buffer[size] = el;
    ++size;
    SiftUp(size - 1);
}

template<class T, class Compare>
T Heap<T, Compare>::ExtractMax() {
    assert(size != 0);
    T max = buffer[0];

    buffer[0] = buffer[size - 1];
    --size;
    SiftDown(0);

    return max;
}

template<class T, class Compare>
const T &Heap<T, Compare>::Peek() const {
    assert(size != 0);
    return buffer[0];
}

template<class T, class Compare>
size_t Heap<T, Compare>::Size() const {
    return size;
}

template<class T, class Compare>
void Heap<T, Compare>::SiftDown(size_t i) {
    size_t left = i * 2 + 1;
    size_t right = i * 2 + 2;

    size_t largest = i;

    if (left < size && cmp(buffer[largest], buffer[left])) {
        largest = left;
    }
    if (right < size && cmp(buffer[largest], buffer[right])) {
        largest = right;
    }

    if (largest != i) {
        std::swap(buffer[largest], buffer[i]);
        SiftDown(largest);
    }
}

template<class T, class Compare>
void Heap<T, Compare>::SiftUp(size_t i) {
    if (i == 0) {
        return;
    }

    size_t parent = (i - 1) / 2;
    if (cmp(buffer[parent], buffer[i])) {
        std::swap(buffer[parent], buffer[i]);
        SiftUp(parent);
    }
}

void MergeArrays(int **arrs, int nArray, int *arrSize, int *sortedArr, int nElement) {
    IsLargeCmpElem cmp;
    Heap<Elem, IsLargeCmpElem> heap(cmp);

    // add to heap first elem from each array
    for (int i = 0; i < nArray; ++i) {
        Elem elemForInsert{arrs[i][0], 0, i};
        heap.Add(elemForInsert);
    }

    // fill sortedArr using ExtractMax
    for (int i = 0; i < nElement; ++i) {
        auto[elValue, elValueIndex, elArrayIndex] = heap.ExtractMax();
        sortedArr[i] = elValue;

        // if array ended
        if (elValueIndex + 1 == arrSize[elArrayIndex]) {
            continue;
        }
        // else we add the next elem from this array
        Elem elemForInsert{arrs[elArrayIndex][elValueIndex + 1], elValueIndex + 1, elArrayIndex};
        heap.Add(elemForInsert);
    }
}
