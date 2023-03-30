#include <iostream>

struct Point {
    int x;
    int y;
};

// sorts in descending order of angles
class IsLessAngle {
public:
    bool operator()(const Point &l, const Point &r) {
        return l.x < r.x ||
               l.x == r.x && l.y > r.y;
    }
};

// sorts in descending order of coords
class IsLessPoint {
public:
    bool operator()(const Point &l, const Point &r) {
        return l.x < r.x ||
               l.x == r.x && l.y < r.y;
    }
};

void InsertionSort(Point *arr, int n) {
    IsLessPoint cmp1;

    Point min = arr[0];
    for (int i = 1; i < n; ++i) {
        if (cmp1(arr[i], min)) {
            min = arr[i];
        }
    }
    std::swap(min, arr[0]);

    IsLessAngle cmp2;

    for (int i = 1; i < n; ++i) {
        Point tmp = arr[i];

        int j = i - 1;
        for (; j >= 1 && cmp2(tmp, arr[j]); --j) {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = tmp;
    }
}

int main() {
    unsigned n = 0;
    std::cin >> n;

    Point *arr = new Point[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i].x >> arr[i].y;
    }

    InsertionSort(arr, n);

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i].x << " " << arr[i].y;
        if (i == n - 1) {
            std::cout << std::endl;
        }
    }

    delete[] arr;
    return 0;
}
