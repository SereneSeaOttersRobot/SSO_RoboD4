#include <DriveMotor.h>
#include <Constants.h>

DriveMotor::DriveMotor(FEHMotorPort motorPort, float voltage, FEHIO::FEHIOPin encoderPin): FEHMotor(motorPort,voltage), AnalogEncoder(encoderPin){
    SetThresholds(Encoder_Low_Threshold,Encoder_High_Threshold);
    AnalogEncoder::ResetCounts();
    //more?
}