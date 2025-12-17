#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <algorithm>

// Подключаем твои сортировки
#include "quicksort.h"

// ================================================
// Вспомогательная функция для проверки сортировки
template<typename T>
bool isSorted(const std::vector<T>& v) {
    return std::is_sorted(v.begin(), v.end());
}

// ================================================
// Тесты для сортировки int
TEST(SortIntTest, EmptyArray) {
    std::vector<int> v;
    sort(v.data(), v.data() + v.size(), std::less<int>{});
    EXPECT_TRUE(isSorted(v));
}

TEST(SortIntTest, SingleElement) {
    std::vector<int> v = {42};
    sort(v.data(), v.data() + v.size(), std::less<int>{});
    EXPECT_TRUE(isSorted(v));
}

TEST(SortIntTest, AlreadySorted) {
    std::vector<int> v = {1,2,3,4,5};
    sort(v.data(), v.data() + v.size(), std::less<int>{});
    EXPECT_TRUE(isSorted(v));
}

TEST(SortIntTest, ReverseSorted) {
    std::vector<int> v = {5,4,3,2,1};
    sort(v.data(), v.data() + v.size(), std::less<int>{});
    EXPECT_TRUE(isSorted(v));
}

TEST(SortIntTest, RandomArray) {
    std::vector<int> v = {3,1,4,1,5,9,2,6,5,3};
    sort(v.data(), v.data() + v.size(), std::less<int>{});
    EXPECT_TRUE(isSorted(v));
}

// ================================================
// Тесты для сортировки double
TEST(SortDoubleTest, RandomArray) {
    std::vector<double> v = {3.1, 2.7, 1.4, 0.5, 2.2};
    sort(v.data(), v.data() + v.size(), std::less<double>{});
    EXPECT_TRUE(isSorted(v));
}

// ================================================
// Тесты для сортировки std::string
TEST(SortStringTest, RandomArray) {
    std::vector<std::string> v = {"banana", "apple", "pear", "orange"};
    sort(v.data(), v.data() + v.size(), std::less<std::string>{});
    EXPECT_TRUE(isSorted(v));
}

TEST(SortStringTest, AlreadySorted) {
    std::vector<std::string> v = {"apple", "banana", "orange", "pear"};
    sort(v.data(), v.data() + v.size(), std::less<std::string>{});
    EXPECT_TRUE(isSorted(v));
}

// ================================================
// Тест с пользовательским типом
struct Person {
    std::string name;
    int age;

    bool operator<(const Person& other) const {
        return age < other.age;
    }
};

TEST(SortCustomTypeTest, RandomPersons) {
    std::vector<Person> people = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}, {"Diana", 20}};
    sort(people.data(), people.data() + people.size(),
         [](const Person& a, const Person& b){ return a.age < b.age; });
    EXPECT_TRUE(std::is_sorted(people.begin(), people.end(),
                               [](const Person& a, const Person& b){ return a.age < b.age; }));
}

// ================================================
// Тест с одинаковыми элементами
TEST(SortIntTest, AllEqual) {
    std::vector<int> v(10, 7);
    sort(v.data(), v.data() + v.size(), std::less<int>{});
    EXPECT_TRUE(isSorted(v));
}

// ================================================
// Тест с отрицательными числами
TEST(SortIntTest, NegativeNumbers) {
    std::vector<int> v = {0, -1, -5, 3, 2};
    sort(v.data(), v.data() + v.size(), std::less<int>{});
    EXPECT_TRUE(isSorted(v));
}

// ================================================
// Тест с threshold = 64 (разделяем на маленькие и большие массивы)
TEST(SortIntTest, SmallArray) {
    std::vector<int> v(10);
    for (int i = 0; i < 10; ++i) v[i] = 10 - i;
    sort(v.data(), v.data() + v.size(), std::less<int>{});
    EXPECT_TRUE(isSorted(v));
}

TEST(SortIntTest, LargeArray) {
    std::vector<int> v(1000);
    for (int i = 0; i < 1000; ++i) v[i] = 1000 - i;
    sort(v.data(), v.data() + v.size(), std::less<int>{});
    EXPECT_TRUE(isSorted(v));
}

// ================================================
// main для запуска тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}