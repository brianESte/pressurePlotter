#include "datapoint.h"
#include <string>

DataPoint::DataPoint(){
    time_stamp = "";
    pressure = 0.0;
}

DataPoint::DataPoint(std::string ts, float p){
    time_stamp = ts;
    pressure = p;
}
