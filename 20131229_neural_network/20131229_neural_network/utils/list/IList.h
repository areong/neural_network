#ifndef UTILS_LIST_ILIST_H_
#define UTILS_LIST_ILIST_H_

template <class T>
class IList {
public:
    /*
        Add item. Still add if exits replicate.
     */
    virtual void add(T t) = 0;
    /*
        Remove item. Do nothing if no match.
     */
    virtual void remove(T t) = 0;
    virtual void removeByIndex(int index) = 0;
    virtual T get(int index) = 0;
    virtual int getIndex(T t) = 0;
    virtual int getLength() = 0;
};

#endif