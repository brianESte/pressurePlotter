#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <string>

/**
 * @brief The DataPoint class
 * Class to hold one data point. Maps a pressure to a time stamp.
 * Probably could have used an actual map or something...
 */
class DataPoint {

public:
    DataPoint();
    DataPoint(std::string, float);

    std::string time_stamp;
    float pressure;
};

#endif // DATAPOINT_H
