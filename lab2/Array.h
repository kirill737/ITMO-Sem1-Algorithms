#pragma once

#include <cstdlib>
#include <utility>


template<typename T>
class Array final {
private:
    T* buff = nullptr;
    int m_size = 0;
    int m_capacity = 0;
    static constexpr double CAPACITY_MUL = 2;
public:
    class Iterator {
    private:
        T* m_ptr;
        int m_index;
        int m_size;
    public:
        Iterator(T* ptr, int index, int size)
            : m_ptr(ptr), m_index(index), m_size(size) {}

        const T& get() const { return m_ptr[m_index]; }
        void set(const T& value) { m_ptr[m_index] = value; }

        void next() { ++m_index; }
        bool hasNext() const { return m_index < m_size; }

        bool operator==(const Iterator& other) const {
            return m_ptr == other.m_ptr && m_index == other.m_index;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        const T& operator*() const { return get(); }
        Iterator& operator++() { next(); return *this; } // Префиксный
        Iterator operator++(int) { Iterator tmp = *this; next(); return tmp; } // Постфиксный

    
    };

    class ConstIterator {
    private:
        const T* m_ptr;
        int m_index;
        int m_size;

    public:
        ConstIterator(const T* ptr, int index, int size)
            : m_ptr(ptr), m_index(index), m_size(size) {}

        const T& get() const { return m_ptr[m_index]; }
        void next() { ++m_index; }
        bool hasNext() const { return m_index < m_size; }

        bool operator==(const Iterator& other) const {
            return m_ptr == other.m_ptr && m_index == other.m_index;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

        const T& operator*() const { return get(); }
        ConstIterator& operator++() { next(); return *this; }
        ConstIterator operator++(int) { ConstIterator tmp = *this; next(); return tmp; }

    };

public:
    Array() : m_capacity(8), m_size(0) {
        buff = static_cast<T*>(std::malloc(sizeof(T) * m_capacity));
    }

    explicit Array(int capacity) : m_capacity(capacity), m_size(0) {
        buff = static_cast<T*>(std::malloc(sizeof(T) * m_capacity));
    }

    Array(const Array& other)
        : m_capacity(other.m_capacity), m_size(other.m_size)
    {
        buff = static_cast<T*>(std::malloc(sizeof(T) * m_capacity));
        for (int i = 0; i < m_size; ++i)
            new (&buff[i]) T(other.buff[i]);
    }

    // Копирование
    // Array& operator=(const Array& other) {
    //     if (this == &other) return *this;

    //     destroyElements();
    //     std::free(buff);

    //     m_capacity = other.m_capacity;
    //     m_size = other.m_size;

    //     buff = static_cast<T*>(std::malloc(sizeof(T) * m_capacity));
    //     for (int i = 0; i < m_size; ++i)
    //         new (&buff[i]) T(other.buff[i]);

    //     return *this;
    // }


    // Перемещение копированием
    // Array& operator=(Array&& other) noexcept {
    //     if (this == &other) return *this;

    //     destroyElements();
    //     std::free(buff);

    //     buff = other.buff;
    //     m_size = other.m_size;
    //     m_capacity = other.m_capacity;

    //     other.buff = nullptr;
    //     other.m_size = 0;
    //     other.m_capacity = 0;

    //     return *this;
    // }


    // Присваивание через copy swap
    // 
    Array& operator=(Array other) {
        std::swap(buff, other.buff);
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
        return *this;
    }


    // Перемещение
    Array(Array&& other) noexcept
        : buff(other.buff), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        other.buff = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    
    ~Array() {
        destroyElements();
        std::free(buff);
    }
    
    int insert(int index, const T& value) {
        if (m_size >= m_capacity) expandArray();
        
        // Сдвиг вправо 
        for (int i = m_size; i > index; --i) {
            new (&buff[i]) T(std::move(buff[i - 1]));
            buff[i - 1].~T();
        }

        new (&buff[index]) T(value);
        ++m_size;
        return index;
    }

    int insert(const T& value) { return insert(m_size, value); }

    void remove(int index) {
        buff[index].~T();
        // Сдвиг влево
        for (int i = index; i < m_size - 1; ++i) {
            new (&buff[i]) T(std::move(buff[i + 1]));
            buff[i + 1].~T();
        }
        --m_size;
    }

    const T& operator[](int index) const { return buff[index]; }
    T& operator[](int index) { return buff[index]; }

    int size() const { return m_size; }
    int capacity() const { return m_capacity; } // Для тестов

    Iterator iterator() { return Iterator(buff, 0, m_size); }
    ConstIterator iterator() const { return ConstIterator(buff, 0, m_size); }

    Iterator reverseIterator() { return Iterator(buff, m_size - 1, m_size); }
    ConstIterator reverseIterator() const { return ConstIterator(buff, m_size - 1, m_size); }

    Iterator begin() { return Iterator(buff, 0, m_size); }
    Iterator end()   { return Iterator(buff, m_size, m_size); }

    ConstIterator begin() const { return ConstIterator(buff, 0, m_size); }
    ConstIterator end()   const { return ConstIterator(buff, m_size, m_size); }

private:
    void expandArray() {
        int newCapacity = static_cast<int>(m_capacity * CAPACITY_MUL);
        if (newCapacity <= m_capacity) newCapacity = m_capacity * 2;

        T* newBlock = static_cast<T*>(std::malloc(sizeof(T) * newCapacity));

        for (int i = 0; i < m_size; ++i) {
            new (&newBlock[i]) T(std::move(buff[i]));
            buff[i].~T();
        }

        std::free(buff);
        buff = newBlock;
        m_capacity = newCapacity;
    }

    void destroyElements() {
        for (int i = 0; i < m_size; ++i)
            buff[i].~T();
    }


};
