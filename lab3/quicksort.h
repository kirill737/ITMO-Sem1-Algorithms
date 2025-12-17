template<typename T, typename Compare>
void insertionSort(T* first, T* last, Compare comp) {
    for (T* i = first + 1; i < last; ++i) {
        T key = std::move(*i);
        T* j = i - 1;

        while (j >= first && comp(key, *j)) {
            *(j + 1) = std::move(*j);
            --j;
        }
        *(j + 1) = std::move(key);
    }
}

template<typename T, typename Compare>
T* median(T* a, T* b, T* c, Compare comp) {
    if (comp(*a, *b)) {
        if (comp(*b, *c)) return b;
        else if (comp(*a, *c)) return c;
        else return a;
    } else {
        if (comp(*a, *c)) return a;
        else if (comp(*b, *c)) return c;
        else return b;
    }
}

template<typename T, typename Compare>
void quickSort(T* first, T* last, Compare comp) {
    while (last - first > 1) {
        T* mid = first + (last - first) / 2;
        T* pivotPtr = median(first, mid, last - 1, comp);

        T pivot = std::move(*pivotPtr);
        std::swap(*pivotPtr, *(last - 1));

        T* i = first;
        T* j = last - 2;

        while (true) {
            while (i <= j && comp(*i, pivot)) ++i;
            while (j >= i && comp(pivot, *j)) --j;
            if (i >= j) break;
            std::swap(*i, *j);
            ++i;
            --j;
        }

        std::swap(*(last - 1), *i);

        // исключение хвостовой рекурсии
        if (i - first < last - (i + 1)) {
            quickSort(first, i, comp);
            first = i + 1;
        } else {
            quickSort(i + 1, last, comp);
            last = i;
        }
    }
}

template<typename T, typename Compare>
void sort(T* first, T* last, Compare comp) {
    const int INSERTION_SORT_THRESHOLD = 64;

    if (last - first <= 1)
        return;

    if (last - first <= INSERTION_SORT_THRESHOLD) {
        insertionSort(first, last, comp);
    } else {
        quickSort(first, last, comp);
    }
}

