#include <iostream>
#include <cassert>

// Возвращает индекс, на который встанет пивот после разделения.
int Partition(int *a, int n) {
  if (n <= 1) {
    return 0;
  }

  // Тут надо реализовать логику выбора опорного элемента

  int pivot = a[n - 1];
  int i = 0;
  int j = n - 2;
  while (i <= j) {
    // Не проверяем, что i < n - 1, т.к. a[n - 1] == pivot.
    for (; a[i] < pivot; ++i) {
    }
    for (; j >= 0 && !(a[j] < pivot); --j) {
    }
    if (i < j) {
      std::swap(a[i++], a[j--]);
    }
  }
  std::swap(a[i], a[n - 1]);
  return i;
}

int FindKStat(int *a, int n, int k) {
  assert(k < n);
  int pivotPos = Partition(a, n);
  if (pivotPos == k) {
    return a[k];
  }

  if (k < pivotPos) {
    return FindKStat(a, pivotPos, k);
  }
  return FindKStat(a + pivotPos + 1, n - pivotPos - 1, k - pivotPos - 1);
}

void CountingSort1(int *a, int n) {
  int max = a[0];
  for (int i = 1; i < n; ++i) {
    max = std::max(max, a[i]);
  }
  int *c = new int[max + 1];
  memset(c, 0, (max + 1) * sizeof(int));

  for (int i = 0; i < n; ++i)
    ++c[a[i]];

  int pos = 0;
  for (int i = 0; i < max + 1; ++i) {
    for (int j = 0; j < c[i]; ++j) {
      a[pos++] = i;
    }
  }
  delete[] c;
}

struct Point {
  int X;
  int Y;
  Point() : X(0), Y(0) {}
  Point(int x, int y) : X(x), Y(y) {}
};

template <class T, class TGetKey>
void CountingSort2(T *a, int n, TGetKey getKey) {
  int max = getKey(a[0]);
  for (int i = 1; i < n; ++i) {
    max = std::max(max, getKey(a[i]));
  }
  int *c = new int[max + 1];
  memset(c, 0, (max + 1) * sizeof(int));

  for (int i = 0; i < n; ++i)
    ++c[getKey(a[i])];

  for (int i = 1; i < max + 1; ++i) {
    c[i] += c[i - 1]; // Концы групп.
  }

  T *b = new T[n];
  for (int i = n - 1; i >= 0; --i) { // Проход с конца.
    b[--c[getKey(a[i])]] = a[i];
  }
  delete[] c;
  for (int i = 0; i < n; ++i) {
    a[i] = b[i];
  }
  memcpy(a, b, n * sizeof(int));
  delete[] b;
}

// 00001111 00010111 10001101 01101100
// 00110011 00010001 10001101 01101100 >> 8 * digit
// &
// 00000000 00000000 00000000 11111111
int main() {
  Point arr[] = {{2, 3}, {3, 0}, {0, 6}, {2, 2}, {7, 1}};
  CountingSort2(arr, 5, [](const Point &pt) { return pt.X; });
  for (int i = 0; i < 5; ++i) {
    std::cout << arr[i].X << ", " << arr[i].Y << "\n";
  }
  return 0;
}