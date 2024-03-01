#include <LightSensor.h>
#include <FEHProteus.h>
#include <fehproteusfirmware\Libraries\FEHIO.h>

#define Light_Counter 1000

LightSensor::LightSensor(FEHIO::FEHIOPin pin, float red_light, float blue_light, float tolerance){
    LightSensor::red_light = red_light;
    LightSensor::blue_light = blue_light;
    LightSensor::tolerance = tolerance;
    AnalogInputPin temp(pin);
    LightSensor::cds_sensor = &temp;
}

LightSensor::LightSensor(FEHIO::FEHIOPin pin){
    LightSensor(pin, Base_Red_Light, Base_Blue_Light, Base_Tolerance);
}

LightSensor::LightSensor(){
    LightSensor(Base_Pin,Base_Red_Light,Base_Blue_Light,Base_Tolerance);
}

void LightSensor::setLights(float red_light, float blue_light){
    LightSensor::red_light = red_light;
    LightSensor::blue_light = blue_light;
}

float LightSensor::lightValue(){
    return cds_sensor->Value();
}

bool LightSensor::detectRed(){
    bool res = false;
    //time counter, if counter exceeds then return false
    //else while should stop with light value in range of red_light
    int counter = 0;
    float lv = LightSensor::lightValue();
    //true if lv is not in light
    bool lvOutR = (lv<red_light-tolerance || red_light+tolerance<lv);
    while (lvOutR && counter <= Light_Counter){
        counter++;
        lv = LightSensor::lightValue();
        lvOutR = (lv<red_light-tolerance || red_light+tolerance<lv);
    };
    res = !lvOutR && counter <= Light_Counter;
    return res;
}

bool LightSensor::detectBlue(){
    bool res = false;
    //time counter, if counter exceeds then return false
    //else while should stop with light value in range of blue_light
    int counter = 0;
    float lv = LightSensor::lightValue();
    //true if lv is not in light
    bool lvOutB = (lv<blue_light-tolerance || blue_light+tolerance<lv);
    while (lvOutB && counter <= Light_Counter){
        counter++;
        lv = LightSensor::lightValue();
        lvOutB = (lv<blue_light-tolerance || blue_light+tolerance<lv);
    };
    res = !lvOutB && counter <= Light_Counter;
    return res;
}

void LightSensor::setTolerance(float tolerance){
    LightSensor::tolerance = tolerance;
}

LightSensor::LColor LightSensor::detectColor(){
    LightSensor::LColor res = LightSensor::LColor::LEMPTY;
    if (LightSensor::detectRed()){
        res = LightSensor::LColor::LRED;
    } else if (LightSensor::detectBlue()){
        res = LightSensor::LColor::LBLUE;
    }
    return res;
}
