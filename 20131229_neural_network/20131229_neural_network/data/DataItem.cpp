#include "DataItem.h"

DataItem::DataItem() {
    y = 0;
    i = new List<int>();
    x_i = new List<double>();
}

DataItem::~DataItem() {
    delete i;
    delete x_i;
}

void DataItem::set_y(double y) {
    this->y = y;
}

void DataItem::add_i_and_x_i(int i, double x_i) {
    this->i->add(i);
    this->x_i->add(x_i);
}

double DataItem::get_y() {
    return y;
}

double DataItem::get_x_i_by_i(int i) {
    // Get the index of i requested.
    int indexOf_i = this->i->getIndex(i);

    // If i is in the list, get the corresponding x_i.
    if (indexOf_i >= 0)
        return x_i->get(indexOf_i);
    // If i is not in the list, neither do x_i, thus return 0.
    else
        return 0;
}