#ifndef UTILS_LIST_LISTBYARRAY_H_
#define UTILS_LIST_LISTBYARRAY_H_

#include "IList.h"

template <class T>
class ListByArray : public IList<T> {
public:
    ListByArray();
    ~ListByArray();

    void add(T t);
    void remove(T t);
    void removeByIndex(int index);
    void clear();
    T get(int index);
    int getIndex(T t);
    void set(int index, T newT);
    int getLength();
private:
    T *list;
    /*
        Length of the array.
     */
    int length;

    /*
        Length of the valid portion of the array.
        Also the index to add to, if it is valid there.
     */
    int lengthValid;
};

// Template definitions should be writen in the header file.

template<class T>
ListByArray<T>::ListByArray() {
    length = 10;
    lengthValid = 0;
    list = new T[length];
}

template<class T>
ListByArray<T>::~ListByArray() {
    delete list;
}

template <class T>
void ListByArray<T>::add(T t) {
    // If space available, add.
    if (lengthValid <= length - 1) {
        list[lengthValid] = t;
        lengthValid += 1;

        // If over half full, expand double.
        if (lengthValid > length / 2) {
            T *listTemp = new T[length * 2];

            // Copy from old.
            for (int i = 0; i < lengthValid; i++)
                listTemp[i] = list[i];

            // Delete old array and point to new.
            delete list;
            list = listTemp;
            length *= 2;
        }
    }
}

template <class T>
void ListByArray<T>::remove(T t) {
    int index = getIndex(t);
    // If item exits, remove it.
    if (index >= 0)
        removeByIndex(index);
}

template <class T>
void ListByArray<T>::removeByIndex(int index) {
    // If index valid.
    if (index >= 0 && index < lengthValid) {
        // Move items after removee afront 1 index.
        for (int i = index; i <= lengthValid - 2; i++)
            list[i] = list[i+1];
        lengthValid -= 1;
    }
}

template <class T>
void ListByArray<T>::clear() {
    lengthValid = 0;
}

template <class T>
T ListByArray<T>::get(int index) {
    // If index valid.
    if (index >= 0 && index < lengthValid)
        return list[index];
    else
        return 0;
}

template <class T>
int ListByArray<T>::getIndex(T t) {
    // Search.
    for (int i = 0; i < lengthValid; i++) {
        // Found.
        if (list[i] == t) {
            return i;
        }
    }
    // Not found, return -1.
    return -1;
}

template <class T>
void ListByArray<T>::set(int index, T newT) {
    // If index valid 
    if (index >= 0 && index < lengthValid)
        list[index] = newT;
}

template <class T>
int ListByArray<T>::getLength() {
    return lengthValid;
}

#endif