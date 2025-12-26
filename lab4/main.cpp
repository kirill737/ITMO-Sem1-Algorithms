#include <iostream>
#include "dice.h"

int main() {
    int res = dice({
        {2, 6, 2},
        {3, 10, 0}
    });
    std::cout << "Res: " << res << std::endl;
    ThrowInfo test1 = strToThrowInfo("2d6-2");
    test1.print();
    std::string resRaw = throwInfoToStr(test1);
    std::cout << resRaw << std::endl;

    DiceSet ds1 = strToDiceSet("2d6+1,1d4,3d10-2");
    for (auto& d : ds1) {
        d.print();
    }

    int res2 = dice("2d6+1,1d4,3d10-2");
    std::cout << "Res: " << res2 << std::endl;
    return 0;
}