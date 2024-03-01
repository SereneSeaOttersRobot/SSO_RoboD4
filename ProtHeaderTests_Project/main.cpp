#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>
#include <Motors.h>
#include <LightSensor.h>

LightSensor Lumy(FEHIO::P1_0);
AnalogInputPin cds(FEHIO::P1_0);
#define VOLTAGE 5.0
//Motors motors(FEHMotor::Motor0,FEHMotor::Motor2,VOLTAGE);

int main (){
    //red light 0.295
    //blue light 0.94
    Lumy.setLights(0.295,0.94);
    Lumy.setTolerance(0.05);
    Lumy.cds_sensor = &cds;
    //testing lightsensor
    while (1) {
        Sleep(0.4);
        LCD.Clear();
        LCD.WriteLine(Lumy.lightValue());
        float lv = Lumy.cds_sensor->Value();
        LCD.WriteLine(lv);
        LCD.WriteLine(cds.Value());
        if (Lumy.detectRed()){
            LCD.WriteLine("Detected Red");
        } else {
            LCD.WriteLine("Haven't detected Red");
        }
    }
}