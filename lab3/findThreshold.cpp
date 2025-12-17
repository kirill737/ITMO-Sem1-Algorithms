#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <functional>
#include <utility>
#include <fstream>
#include "quicksort.h"

std::vector<int> generateRandomArray(
    size_t size,
    int minValue = 0,
    int maxValue = 1'000'000
) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(minValue, maxValue);

    std::vector<int> data(size);
    for (auto& x : data)
        x = dist(rng);

    return data;
}


using steady_clock_t = std::chrono::steady_clock;

std::pair<long long, long long>
measureSortTime(const std::vector<int>& data) {
    const int REPEAT = 10000;

    auto startInsert = steady_clock_t::now();
    for (int i = 0; i < REPEAT; ++i) {
        auto copy = data;
        insertionSort(
            copy.data(),
            copy.data() + copy.size(),
            std::less<int>{}
        );
    }
    auto endInsert = steady_clock_t::now();


    auto startQuick = steady_clock_t::now();
    for (int i = 0; i < REPEAT; ++i) {
        auto copy = data;
        quickSort(
            copy.data(),
            copy.data() + copy.size(),
            std::less<int>{}
        );
    }
    auto endQuick = steady_clock_t::now();

    long long insertTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endInsert - startInsert).count() / REPEAT;
    long long quickTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endQuick - startQuick).count() / REPEAT;

    return {insertTime, quickTime};
}

int main() {
    const int RUNS = 7;
    const int MAX_N = 150;

    std::ofstream fout("result.csv");
    if (!fout) {
        std::cerr << "Ошибка при открытии файла result.csv\n";
        return 1;
    }

    fout << "N,insert_ns,quick_ns\n";

    for (int n = 1; n <= MAX_N; ++n) {
        long long sumInsert = 0;
        long long sumQuick = 0;

        for (int r = 0; r < RUNS; ++r) {
            auto data = generateRandomArray(n);
            auto [insertTime, quickTime] = measureSortTime(data);

            sumInsert += insertTime;
            sumQuick  += quickTime;
        }

        long long avgInsert = sumInsert / RUNS;
        long long avgQuick  = sumQuick  / RUNS;

        fout << n << "," << avgInsert << "," << avgQuick << "\n";
        std::cout << n << "," << avgInsert << "," << avgQuick << std::endl;
    }

    fout.close();
    std::cout << "Результаты сохранены в result.csv\n";
    return 0;
}
