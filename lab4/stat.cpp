#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include "dice.h"

std::map<int, int> simulate(const DiceSet& diceSet, int N) {
    std::map<int, int> freq;

    for (int i = 0; i < N; ++i)
        freq[dice(diceSet)]++;
    return freq;
}

void saveCSV(const std::string& filename, const std::map<int, int>& freq, int N)
{
    std::ofstream out(filename);
    out << "value,prob\n";
    for (const auto& [value, count] : freq) {
        out << value << "," << static_cast<double>(count) / N << "\n";
    }
}

int main() {
    const int N = 100000;

    std::vector<std::string> tests = {"1d6", "2d6", "3d6", "1d10", "2d10", "3d10", "10d10", "10d100"};

    for (const auto& test : tests) {
        auto freq = simulate(strToDiceSet(test), N);
        saveCSV("../stat/" + test + ".csv", freq, N);
        std::cout << "Saved " << test << ".csv\n";
    }

    return 0;
}
