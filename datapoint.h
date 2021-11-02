#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <string>

class DataPoint {

public:
    DataPoint();
    DataPoint(std::string, float);

    std::string time_stamp;
    float pressure;
};

#endif // DATAPOINT_H
