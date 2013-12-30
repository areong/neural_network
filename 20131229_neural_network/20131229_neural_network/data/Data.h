#ifndef DATA_DATA_H_
#define DATA_DATA_H_

#include "../utils/list/List.h"

class DataItem;

class Data {
public:
    Data();
    ~Data();

private:
    /*
        Rows of data.
     */
    List<DataItem *> *dataRows;
};

#endif