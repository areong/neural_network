#include "Data.h"
#include "DataItem.h"

Data::Data() {
    dataItems = new List<DataItem *>();
}

Data::~Data() {
    if (dataItems->getLength() > 0)
        for (int i = 0; i < dataItems->getLength(); i++)
            delete dataItems->get(i);
    delete dataItems;

    if (dataItemTemp != 0)
        delete dataItemTemp;
}

void Data::startAddingOneItem() {
    // Set state.
    isAddingOneItem = true;

    // Create item to add.
    dataItemTemp = new DataItem();
}

void Data::finishAddingOneItem() {
    // Add the item to dataItems.
    dataItems->add(dataItemTemp);

    // Set state.
    isAddingOneItem = false;
}

void Data::add_y(double y) {
    // If is adding one item.
    if (isAddingOneItem) {
        // Set y.
        dataItemTemp->set_y(y);
    }
}

void Data::add_i_and_x_i(int i, double x_i) {
    // If is adding one item.
    if (isAddingOneItem) {
        // Add i and x_i.
        dataItemTemp->add_i_and_x_i(i, x_i);
    }
}

int Data::getNumItems() {
    return dataItems->getLength();
}

double Data::get_y_byIndex(int index) {
    return dataItems->get(index)->get_y();
}

double Data::get_x_i_byIndexAnd_i(int index, int i) {
    // DataItem::get_x_i_by_i(int i) returns 0 if x_i not found.
    return dataItems->get(index)->get_x_i_by_i(i);
}