#include <CdSensor.h>
#include <FEHIO.h>
#include <Constants.h>

bool CdSensor::notBlue(){
    float light = Value();
    return (light < CdS_Blue_Lower || CdS_Blue_Upper < light);
}

bool CdSensor::notRed(){
    float light = Value();
    return (light< CdS_Red_Lower || CdS_Blue_Upper < light);
}