#include <DriveMotor.h>
#include <Constants.h>

DriveMotor::DriveMotor(FEHMotorPort motorPort, float voltage, FEHIO::FEHIOPin encoderPin): FEHMotor(motorPort,voltage), AnalogEncoder(encoderPin){
    SetThresholds(Encoder_Low_Threshold,Encoder_High_Threshold);
    ResetCounts();
    this->currentPercent = 0.0;
    
}

void  DriveMotor::SetPercent(float percent){
    this->currentPercent = percent;
    FEHMotor::SetPercent(percent);
}

void DriveMotor::Stop(){
    FEHMotor::Stop();
    this->currentPercent = 0.0;
}

void DriveMotor::ResetCounts(){
    AnalogEncoder::ResetCounts();
}

int DriveMotor::Counts(){
    return AnalogEncoder::Counts();
}

float DriveMotor::CurrentPercent(){
    return this->currentPercent;
}