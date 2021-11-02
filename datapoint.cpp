#include "datapoint.h"
#include <string>

// default / blank constructor
DataPoint::DataPoint(){
    time_stamp = "";
    pressure = 0.0;
}

/**
 * @brief DataPoint::DataPoint
 * @param ts        Timestamp for the datapoint
 * @param p         Pressure [mbar] for the datapoint
 */
DataPoint::DataPoint(std::string ts, float p){
    time_stamp = ts;
    pressure = p;
}
