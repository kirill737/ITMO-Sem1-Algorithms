#include "Array.h"
#include <gtest/gtest.h>
#include <string>

//  Проверка вставки и размера 
TEST(ArrayTest, InsertAndSize) {
    Array<int> arr;
    EXPECT_EQ(arr.size(), 0);

    arr.insert(10);
    arr.insert(20);
    arr.insert(30);

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
}

//  Проверка вставки в середину 
TEST(ArrayTest, InsertAtIndex) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(3);
    arr.insert(1, 2);

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}

//  Проверка удаления 
TEST(ArrayTest, RemoveElement) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3);

    arr.remove(1);

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 3);
}


//  Проверка доступа через оператор [] 
TEST(ArrayTest, AccessOperator) {
    Array<int> arr;
    arr.insert(5);
    arr.insert(10);

    arr[0] = 15;
    EXPECT_EQ(arr[0], 15);
    EXPECT_EQ(arr[1], 10);

    const Array<int>& carr = arr;
    EXPECT_EQ(carr[0], 15);
    EXPECT_EQ(carr[1], 10);
}

//  Проверка копирования конструктора и присваивания 
TEST(ArrayTest, CopyConstructorAndAssignment) {
    Array<int> a;
    a.insert(1);
    a.insert(2);

    Array<int> b(a);
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);

    Array<int> c;
    c = b;
    EXPECT_EQ(c.size(), 2);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
}

//  Проверка перемещения конструктора и присваивания 
TEST(ArrayTest, MoveConstructorAndAssignment) {
    Array<int> a;
    a.insert(10);
    a.insert(100);

    Array<int> b(std::move(a));
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], 10);
    EXPECT_EQ(b[1], 100);
    EXPECT_EQ(a.size(), 0);

    Array<int> c;
    c = std::move(b);
    EXPECT_EQ(c.size(), 2);
    EXPECT_EQ(c[0], 10);
    EXPECT_EQ(c[1], 100);
    EXPECT_EQ(b.size(), 0);
}

//  Проверка работы обычного итератора 
TEST(ArrayTest, Iterator) {
    Array<int> arr;
    for (int i = 1; i <= 5; ++i) arr.insert(i);

    int sum = 0;
    for (auto it = arr.iterator(); it.hasNext(); it.next()) {
        sum += it.get();
    }
    EXPECT_EQ(sum, 15);
}

//  Проверка работы константного итератора 
TEST(ArrayTest, ConstIterator) {
    Array<int> arr;
    for (int i = 1; i <= 3; ++i) arr.insert(i);

    const Array<int>& cArr = arr;
    int sum = 0;
    for (auto it = cArr.iterator(); it.hasNext(); it.next()) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

//  Проверка reverseIterator 
TEST(ArrayTest, ReverseIterator) {
    Array<int> arr;
    for (int i = 1; i <= 3; ++i) arr.insert(i);

    int expected = 3;
    for (auto it = arr.reverseIterator(); it.hasNext(); it.next()) {
        EXPECT_EQ(it.get(), expected--);
    }
}

//  Проверка begin()/end() 
TEST(ArrayTest, BeginEndIterators) {
    Array<int> arr;
    for (int i = 1; i <= 3; ++i) arr.insert(i);

    int expected = 1;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

//  Проверка расширения массива 
TEST(ArrayTest, CapacityExpansion) {
    Array<int> arr(2); // маленькая начальная емкость
    arr.insert(1);
    arr.insert(2);
    arr.insert(3); // должно стать в 2 раза больше

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.capacity(), 4);
    EXPECT_EQ(arr[2], 3);
}

TEST(ArrayTest, NoCapacityExpansion) {
    Array<int> arr;
    arr.insert(1);
    arr.insert(2);
    arr.insert(3); // не должно измениться, так было сразу 8

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.capacity(), 8);
    EXPECT_EQ(arr[2], 3);
}

//  Проверка с сложным типом
TEST(ArrayTest, CustomType) {
    Array<std::string> arr;
    arr.insert("hello");
    arr.insert("world");

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], "hello");
    EXPECT_EQ(arr[1], "world");

    arr[1] = "hi";
    EXPECT_EQ(arr[1], "hi");
}


//  Проверка вставки и размера 
TEST(ArrayTest, InsertAndSizeStr) {
    Array<std::string> arr;
    EXPECT_EQ(arr.size(), 0);

    arr.insert("10");
    arr.insert("20");
    arr.insert("30");

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], "10");
    EXPECT_EQ(arr[1], "20");
    EXPECT_EQ(arr[2], "30");
}

//  Проверка вставки в середину 
TEST(ArrayTest, InsertAtIndexStr) {
    Array<std::string> arr;
    arr.insert("1");
    arr.insert("3");
    arr.insert(1, "2");

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr[0], "1");
    EXPECT_EQ(arr[1], "2");
    EXPECT_EQ(arr[2], "3");
}

//  Проверка удаления 
TEST(ArrayTest, RemoveElementStr) {
    Array<std::string> arr;
    arr.insert("1");
    arr.insert("2");
    arr.insert("3");

    arr.remove(1);

    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], "1");
    EXPECT_EQ(arr[1], "3");
}

//  Проверка доступа через оператор [] 
TEST(ArrayTest, AccessOperatorStr) {
    Array<std::string> arr;
    arr.insert("5");
    arr.insert("10");

    arr[0] = "15";
    EXPECT_EQ(arr[0], "15");
    EXPECT_EQ(arr[1], "10");

    const Array<std::string>& carr = arr;
    EXPECT_EQ(carr[0], "15");
    EXPECT_EQ(carr[1], "10");
}

//  Проверка копирования конструктора и присваивания 
TEST(ArrayTest, CopyConstructorAndAssignmentStr) {
    Array<std::string> a;
    a.insert("1");
    a.insert("2");

    Array<std::string> b(a);
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], "1");
    EXPECT_EQ(b[1], "2");

    Array<std::string> c;
    c = b;
    EXPECT_EQ(c.size(), 2);
    EXPECT_EQ(c[0], "1");
    EXPECT_EQ(c[1], "2");
}

//  Проверка перемещения конструктора и присваивания 
TEST(ArrayTest, MoveConstructorAndAssignmentStr) {
    Array<std::string> a;
    a.insert("10");
    a.insert("100");

    Array<std::string> b(std::move(a));
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], "10");
    EXPECT_EQ(b[1], "100");
    EXPECT_EQ(a.size(), 0);

    Array<std::string> c;
    c = std::move(b);
    EXPECT_EQ(c.size(), 2);
    EXPECT_EQ(c[0], "10");
    EXPECT_EQ(c[1], "100");
    EXPECT_EQ(b.size(), 0);
}

//  Проверка работы обычного итератора 
TEST(ArrayTest, IteratorStr) {
    Array<std::string> arr;
    arr.insert("a");
    arr.insert("b");
    arr.insert("c");

    std::string result;
    for (auto it = arr.iterator(); it.hasNext(); it.next()) {
        result += it.get();
    }
    EXPECT_EQ(result, "abc");
}

//  Проверка работы константного итератора 
TEST(ArrayTest, ConstIteratorStr) {
    Array<std::string> arr;
    arr.insert("a");
    arr.insert("b");
    arr.insert("c");

    const Array<std::string>& cArr = arr;
    std::string result;
    for (auto it = cArr.iterator(); it.hasNext(); it.next()) {
        result += *it;
    }
    EXPECT_EQ(result, "abc");
}

//  Проверка reverseIterator 
TEST(ArrayTest, ReverseIteratorStr) {
    Array<std::string> arr;
    arr.insert("a");
    arr.insert("b");
    arr.insert("c");

    std::string expected[] = {"c", "b", "a"};
    int i = 0;
    for (auto it = arr.reverseIterator(); it.hasNext(); it.next()) {
        EXPECT_EQ(it.get(), expected[i++]);
    }
}

//  Проверка begin()/end() 
TEST(ArrayTest, BeginEndIteratorsStr) {
    Array<std::string> arr;
    arr.insert("a");
    arr.insert("b");
    arr.insert("c");

    std::string expected[] = {"a", "b", "c"};
    int i = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        EXPECT_EQ(*it, expected[i++]);
    }
}

//  Проверка расширения массива 
TEST(ArrayTest, CapacityExpansionStr) {
    Array<std::string> arr(2);
    arr.insert("1");
    arr.insert("2");
    arr.insert("3");

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.capacity(), 4);
    EXPECT_EQ(arr[2], "3");
}

TEST(ArrayTest, NoCapacityExpansionStr) {
    Array<std::string> arr;
    arr.insert("1");
    arr.insert("2");
    arr.insert("3");

    EXPECT_EQ(arr.size(), 3);
    EXPECT_EQ(arr.capacity(), 8);
    EXPECT_EQ(arr[2], "3");
}

