#include <iostream>

struct Point {
  int X;
  int Y;
  Point() : X(0), Y(0) {}
  Point(int x, int y) : X(x), Y(y) {}
};

// 1
bool operator<(const Point &l, const Point &r) { return l.X < r.X; }

// 2
bool IsLessByX(const Point &l, const Point &r) { return l.X < r.X; }

bool IsLessByY(const Point &l, const Point &r) { return l.Y < r.Y; }

// 3
template <class T> bool IsLessDefault(const T &l, const T &r) { return l < r; }

// 4
class IsLessCmpByDX {
public:
  IsLessCmpByDX(const Point &_reper) : reper(_reper) {}

  bool operator()(const Point &l, const Point &r) {
    return std::abs(l.X - reper.X) < std::abs(r.X - reper.X);
  }

private:
  Point reper;
};

// 5
template <class T> class IsLessCmpDefault {
public:
  bool operator()(const T &l, const T &r) { return l < r; }
};

template <class T, class Compare = IsLessCmpDefault<T>>
void MegaSort(T *arr, int l, int r,
              Compare cmp = IsLessCmpDefault<
                  T>() /*bool isLess(const T&, const T&) = IsLessDefault*/) {
  for (int i = l; i < r; ++i) {
    for (int j = l; j < r; ++j) {
      if (cmp(arr[j + 1], arr[j])) {
        std::swap(arr[j + 1], arr[j]);
      }
    }
  }
}

int main() {
  int intArr[5] = {3, 4, -1, 2, 0};
  MegaSort(intArr, 0, 4);

  char charArr[5] = {3, 4, -1, 2, 0};
  MegaSort(charArr, 0, 4);

  Point pointArr[5] = {{3, 4}, {-1, 2}, {0, 6}, {2, 2}, {-7, 10}};

  MegaSort(pointArr, 0, 4);
  MegaSort(pointArr, 0, 4, IsLessByX);
  MegaSort(pointArr, 0, 4,
           [](const Point &l, const Point &r) { return l.Y < r.Y; });
  IsLessCmpByDX cmp({0, 5});
  MegaSort(pointArr, 0, 4, cmp);

  for (int i = 0; i < 5; ++i) {
    std::cout << "(" << pointArr[i].X << ", " << pointArr[i].Y << "), ";
  }
  std::cout << "\n";

  return 0;
}

/*
template <class T, class Compare>
class Heap {
public:
        Heap( Compare cmp );
        Heap( const T* arr, int size, Compare cmp );

        void Add( const T& );
        T ExtractMax();
        const T& Peek() const;
        size_t Size() const;

private:
        Compare cmp;
        T* buffer;
        size_t bufferSize;
        size_t size;
};

vector<vector<int>> arrs;
struct Elem {
        int Value;
        int ValueIndex;
        int ArrayIndex;
};

q 13; r 10;           p 5; t 32
struct User {
        char Id;
        size_t Count;
};
*/
