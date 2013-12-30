#ifndef DATA_DATAITEM_H_
#define DATA_DATAITEM_H_

#include "../utils/list/List.h"

class DataItem {
public:
    DataItem();
    ~DataItem();
    void set_y(double y);
    void add_i_and_x_i(int i, double x_i);

    double get_y();
    double get_x_i_by_i(int i);

private:
    double y;
    List<int> *i;
    List<double> *x_i;
};

#endif