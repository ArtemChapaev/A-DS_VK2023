// 5.2. Современники.
// Группа людей называется современниками если был такой момент, когда они могли собраться вместе.
// Для этого в этот момент каждому из них должно было  уже исполниться 18 лет, но ещё не исполниться 80 лет.
// Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников. В день 18летия человек
// уже может принимать участие в собраниях, а в день 80летия и в день смерти уже не может.

// Замечание. Человек мог не дожить до 18-летия, либо умереть в день 18-летия.
// В этих случаях принимать участие в собраниях он не мог.


// Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием в
// виде шаблонной функции.
// Решение должно поддерживать передачу функции сравнения снаружи.
// Общее время работы алгоритма O(n log n).


#include <iostream>
#include <sstream>

template<class T>
class IsLessCmpDefault;

struct Date;

int PickMaxContemporaries(Date *lifeDates, int size);

int handleDates(Date *lifeDates, int size, Date *timestamps);

template<class T, class Compare = IsLessCmpDefault<T>>
void MergeSort(T *arr, int size, Compare cmp = IsLessCmpDefault<T>());

template<class T, class Compare>
void Merge(T *arr, int left, int mid, int right, Compare cmp);

template<class T>
class IsLessCmpDefault {
public:
    bool operator()(const T &l, const T &r) {
        return l < r;
    }
};

struct Date {
    int day;
    int month;
    int year;
    // event can be -1 or 1. -1 -> person is 80 or died, 1 -> person is 18
    int event;

    Date() : day(-1), month(-1), year(-1), event(0) {}
};

class IsLessCmpDate {
public:
    bool operator()(const Date &l, const Date &r) {
        if (l.year < r.year ||
            l.year == r.year && l.month < r.month ||
            l.year == r.year && l.month == r.month && l.day < r.day ||
            l.year == r.year && l.month == r.month && l.day == r.day && l.event < r.event) {
            return true;
        }
        return false;
    }
};

int main() {
    int nPerson = 0;
    std::cin >> nPerson;

    Date *lifeDates = new Date[nPerson * 2];
    for (int i = 0; i < nPerson; ++i) {
        std::cin >> lifeDates[i * 2].day >> lifeDates[i * 2].month >> lifeDates[i * 2].year;
        std::cin >> lifeDates[i * 2 + 1].day >> lifeDates[i * 2 + 1].month >> lifeDates[i * 2 + 1].year;
    }

    int result = PickMaxContemporaries(lifeDates, nPerson * 2);
    std::cout << result;

    delete[] lifeDates;
    return 0;
}

int PickMaxContemporaries(Date *lifeDates, int size) {
    // array where we will store all timestamps
    Date *timestamps = new Date[size];
    int nTimestamp = handleDates(lifeDates, size, timestamps);

    IsLessCmpDate cmp;
    MergeSort(timestamps, nTimestamp, cmp);

    // here is "scanner" for search of max contemporaries
    int maxContems = 0;
    int currContems = 0;
    for (int i = 0; i < nTimestamp; ++i) {
        currContems += timestamps[i].event;
        if (currContems > maxContems) {
            maxContems = currContems;
        }
    }
    std::cout << std::endl;

    delete[] timestamps;
    return maxContems;
}

int handleDates(Date *lifeDates, int size, Date *timestamps) {
    // for counting current index in timestamps array
    int indexT = 0;

    // checking all dates using task's condition
    for (int i = 0; i < size; i += 2) {
        int yearDelta = lifeDates[i + 1].year - lifeDates[i].year;
        int monthDelta = lifeDates[i + 1].month - lifeDates[i].month;
        int dayDelta = lifeDates[i + 1].day - lifeDates[i].day;

        // if person wasn't 18 years old we don't add info in timestamps
        if (yearDelta < 18) {
            continue;
        }

        // if person wasn't 18 years old we don't add info in timestamps
        if (yearDelta == 18) {
            if (monthDelta > 0 || monthDelta == 0 && dayDelta > 0) {
                Date dayOf18 = lifeDates[i];
                dayOf18.year += 18;

                timestamps[indexT] = dayOf18;
                timestamps[indexT + 1] = lifeDates[i + 1];

                timestamps[indexT].event = 1;
                timestamps[indexT + 1].event = -1;
                indexT += 2;
            }
            // anyway we finish this iteration if yearDelta == 18
            continue;
        }

        // if we here then person was 18 years old
        Date dayOf18 = lifeDates[i];
        dayOf18.year += 18;
        timestamps[indexT] = dayOf18;
        timestamps[indexT].event = 1;

        if (yearDelta < 80) {
            timestamps[indexT + 1] = lifeDates[i + 1];
            timestamps[indexT + 1].event = -1;
            indexT += 2;
            continue;
        }

        if (yearDelta == 80) {
            if (monthDelta < 0 || monthDelta == 0 && dayDelta < 0) {
                timestamps[indexT + 1] = lifeDates[i + 1];
                timestamps[indexT + 1].event = -1;
                indexT += 2;
                // we finish this iteration only if person wasn't 80 years old
                continue;
            }
        }

        // if we here then person was 80 years old
        Date dayOf80 = lifeDates[i];
        dayOf80.year += 80;
        timestamps[indexT + 1] = dayOf80;
        timestamps[indexT + 1].event = -1;
        indexT += 2;
    }

    // we return amount of timestamps
    return indexT;
}

template<class T, class Compare>
void MergeSort(T *arr, int size, Compare cmp) {
    // action of n*=2 is similar with log(size)
    for (int n = 1; n < size; n *= 2) {
        // i += 2 because i show length of 1 array. and we must skip 2 "arrays"
        for (int i = 0; i < size / n; i += 2) {
            int left = i * n;
            int mid = (i + 1) * n;
            int right = (i + 2) * n > size ? size : (i + 2) * n;

            Merge(arr, left, mid, right, cmp);
        }
    }
}

template<class T, class Compare>
void Merge(T *arr, int left, int mid, int right, Compare cmp) {
    // auxiliary array for storage left array
    T *temp = new T[mid - left];
    for (int i = 0; i < mid - left; ++i) {
        temp[i] = arr[left + i];
    }
    int lIt = 0;
    int rIt = 0;

    // we ese property next property: if k < mid - left, then array[left+k] == temp[k]
    // leave loop if one of arrays end
    while (left + lIt < mid && mid + rIt < right) {
        if (cmp(temp[lIt], arr[mid + rIt])) {
            arr[left + lIt + rIt] = temp[lIt];
            ++lIt;
        } else {
            arr[left + lIt + rIt] = arr[mid + rIt];
            ++rIt;
        }
    }

    // if we leave first loop because right array ended
    while (left + lIt < mid) {
        arr[left + lIt + rIt] = temp[lIt];
        ++lIt;
    }

    // if we leave first loop because left array ended
    while (mid + rIt < right) {
        arr[left + lIt + rIt] = arr[mid + rIt];
        ++rIt;
    }

    delete[] temp;
}
