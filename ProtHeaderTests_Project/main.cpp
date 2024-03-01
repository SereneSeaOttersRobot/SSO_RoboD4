#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>
#include <customLibs\Motors.h>
#include <customLibs\LightSensor.h>

LightSensor Lumy(FEHIO::P0_0);
#define VOLTAGE 5.0
Motors motors(FEHMotor::Motor0,FEHMotor::Motor1,VOLTAGE);

int main (){
    
}