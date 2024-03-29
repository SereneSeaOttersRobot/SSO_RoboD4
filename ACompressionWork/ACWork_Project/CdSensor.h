#ifndef CDSENSOR_H
#define CDSENSOR_H

#include <FEHIO.h>

class CdSensor: public AnalogInputPin {
    using AnalogInputPin::AnalogInputPin;

    public:

    /**
     * Returns true if light is not Blue.
    */
    bool notBlue();
    /**
     * Returns true if light is not Red
    */
    bool notRed();

};

#endif