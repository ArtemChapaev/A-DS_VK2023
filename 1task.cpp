// 1.3.
// Если в числе содержится только один бит со значением 1, записать в выходной поток OK. Иначе записать FAIL

// Во всех задачах необходимо использование битовых операций.
// Использование арифметических операций запрещено.
// Входное число лежит в диапазоне 0..232 -1 и вводится в десятичном виде.


#include <iostream>

bool CheckBitAmount(unsigned num);

int main() {
    unsigned num = 0;
    std::cin >> num;

    bool res = CheckBitAmount(num);
    std::cout << (res ? "OK\n" : "FAIL\n");
    return 0;
}

bool CheckBitAmount(unsigned num) {
    unsigned counter = 0;
    while (num) {
        if (num & 1) { // checking last bit
            ++counter;
        }
        num >>= 1;
    }
    return counter == 1;
}
