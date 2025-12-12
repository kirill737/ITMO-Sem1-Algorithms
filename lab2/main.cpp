#include <iostream>
#include "Array.h"
#include <windows.h>

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    Array<int> a;
    a.insert(1);
    std::cout << a[0] << std::endl;
    Array<int> b = a;
    std::cout << b[0] << std::endl;
    return 0;
}
