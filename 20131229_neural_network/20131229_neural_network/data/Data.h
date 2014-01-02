#ifndef DATA_DATA_H_
#define DATA_DATA_H_

#include "../utils/list/List.h"

class DataItem;

class Data {
public:
    Data();
    ~Data();

    /*
        Call before adding one item .
     */
    void startAddingOneItem();

    /*
        Call after adding one item.
     */
    void finishAddingOneItem();

    /*
        Add y to the current item.
        Call after startAddingOneItem() and
        before finishAddingOneItem().
     */
    void add_y(double y);

    /*
        Add i and x_i to the current item.
        Call after startAddingOneItem() and
        before finishAddingOneItem().
     */
    void add_i_and_x_i(int i, double x_i);

    int getNumItems();
    double get_y_byIndex(int index);
    double get_x_i_byIndexAnd_i(int index, int i);

    void addDataItem(DataItem *dataItem);
    DataItem *getDataItem(int index);
    
private:
    /*
        DataItems to store items of data.
     */
    List<DataItem *> *dataItems;

    /*
        State of whether it is adding one item.
     */
    bool isAddingOneItem;

    /*
        Temporary DataItem used when adding one item of data.
     */
    DataItem *dataItemTemp;
};

#endif