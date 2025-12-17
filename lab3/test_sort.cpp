#include <gtest/gtest.h>
#include "quicksort.h"

TEST(QuickSortTest, Integers) {
    int arr[] = {4, 2, 5, 1, 6, 3};
    sort(arr, arr + 6, [](int a, int b){ return a < b; });

    for (int i = 1; i <= 6; ++i)
        EXPECT_EQ(arr[i - 1], i);
}

TEST(QuickSortTest, EmptyArray) {
    int* arr = nullptr;
    sort(arr, arr, [](int a, int b){ return a < b; });
    SUCCEED();
}


TEST(QuickSortTest, SingleElement) {
    int arr[] = {737};
    sort(arr, arr + 1, [](int a, int b){ return a < b; });
    EXPECT_EQ(arr[0], 737);
}

TEST(QuickSortTest, Strings) {
    std::string arr[] = {"banana", "apple", "pear", "orange"};
    sort(arr, arr + 4, [](const std::string& a, const std::string& b){ return a < b; });
    EXPECT_EQ(arr[0], "apple");
    EXPECT_EQ(arr[1], "banana");
    EXPECT_EQ(arr[2], "orange");
    EXPECT_EQ(arr[3], "pear");
}

TEST(QuickSortTest, DescendingOrder) {
    int arr[] = {1, 2, 3, 4, 5};
    sort(arr, arr + 5, [](int a, int b){ return a > b; }); // обратный порядок
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(arr[i], 5 - i);
}
