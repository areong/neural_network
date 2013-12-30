#include "Data.h"
#include "DataItem.h"

Data::Data() {
    dataRows = new List<DataItem *>();
}

Data::~Data() {
    if (dataRows->getLength() > 0)
        for (int i = 0; i < dataRows->getLength(); i++)
            delete dataRows->get(i);
    delete dataRows;
}