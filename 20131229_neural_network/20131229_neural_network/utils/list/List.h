#ifndef UTILS_LIST_LIST_H_
#define UTILS_LIST_LIST_H_

#include "IList.h"
#include "ListByArray.h"

template <class T>
class List : public IList<T> {
public:
    List();
    ~List();

    void add(T t);
    void remove(T t);
    void removeByIndex(int index);
    void clear();
    T get(int index);
    int getIndex(T t);
    void set(int index, T newT);
    int getLength();
private:
    ListByArray<T> *listReal;
};

// Template definitions should be writen in the header file.

template <class T>
List<T>::List() {
    listReal = new ListByArray<T>();
}

template <class T>
List<T>::~List() {
    delete listReal;
}

template <class T>
void List<T>::add(T t) {
    listReal->add(t);
}

template <class T>
void List<T>::remove(T t) {
    listReal->remove(t);
}

template <class T>
void List<T>::removeByIndex(int index) {
    listReal->removeByIndex(index);
}

template <class T>
void List<T>::clear() {
    listReal->clear();
}

template <class T>
T List<T>::get(int index) {
    return listReal->get(index);
}

template <class T>
int List<T>::getIndex(T t) {
    return listReal->getIndex(t);
}

template <class T>
void List<T>::set(int index, T newT) {
    listReal->set(index, newT);
}

template <class T>
int List<T>::getLength() {
    return listReal->getLength();
}

#endif