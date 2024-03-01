#ifndef LightSensor_H
#define LightSensor_H
#include <fehproteusfirmware\Libraries\FEHIO.h>

#define Base_Blue_Light 0.94
#define Base_Red_Light 0.295
#define Base_Tolerance 0.05
#define Base_Pin FEHIO::FEHIOPin::P0_0

class LightSensor{
    private:
    float red_light;
    float blue_light;
    float tolerance;
    AnalogInputPin * cds_sensor;
    public:
    LightSensor();
    LightSensor(FEHIO::FEHIOPin pin, float red_light, float blue_light, float tolerance);
    LightSensor(FEHIO::FEHIOPin pin);
    void setLights(float red_light, float blue_light);
    float lightValue();
    bool detectRed();
    bool detectBlue();
    void setTolerance(float tolerance);
    enum LColor{
        RED, BLUE, EMPTY
        //expandable
    };
    LightSensor::LColor detectColor();
};

#endif