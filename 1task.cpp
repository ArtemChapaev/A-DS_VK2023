#include <iostream>

int main() {
    unsigned num = 0;
    std::cin >> num;

    unsigned counter = 0;
    while (num) {
        if (num & 1) { // checking last bit
            ++counter;
        }
        num >>= 1;
    }

    std::cout << (counter == 1 ? "OK\n" : "FAIL\n");
    return 0;
}