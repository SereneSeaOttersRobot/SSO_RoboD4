#include <DriveMotor.h>
#include <Constants.h>
#include <FEHUtility.h>

DriveMotor::DriveMotor(FEHMotorPort motorPort, float voltage, FEHIO::FEHIOPin encoderPin): FEHMotor(motorPort,voltage), AnalogEncoder(encoderPin){
    SetThresholds(Encoder_Low_Threshold,Encoder_High_Threshold);
    ResetCounts();
    this->currentPercent = 0.0;
    this->lastCounts = 0;
    this->lastTime = 0.0;
    this->sumError = 0.0;
    this->preError = 0.0;
    this->Pconst = 0.0;
    this->Iconst = 0.0;
    this->Dconst = 0.0;
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

void DriveMotor::ResetPIDVars(double startTime){
    this->lastCounts = 0;
    this->lastTime = startTime;
    this->sumError = 0.0;
    this->preError = 0.0;
    AnalogEncoder::ResetCounts;
}

void DriveMotor::setPIDConstants(double Pvar, double Ivar, double Dvar){
    this->Pconst = Pvar;
    this->Iconst = Ivar;
    this->Dconst = Dvar;
}

float DriveMotor::PIDAdjustment(double expectedSpeed){
    int currentCounts = Counts();
    double currentTime = TimeNow();
    int dCounts = currentCounts - lastCounts;
    double dTime = currentTime - lastTime;
    double actualSpeed = (1.0/TICKS_PER_INCH) * (dCounts/dTime);
    double error = expectedSpeed - actualSpeed;
    double PTerm = error*Pconst;
    this->sumError += error;
    double ITerm = sumError*Iconst;
    double DTerm = (error - preError)*Dconst;
    float result = currentPercent+PTerm+ITerm+DTerm;
    preError = error;
    lastCounts = currentCounts;
    lastTime = currentTime;
}

DriveTrain::DriveTrain(DriveMotor leftMotor, DriveMotor rightMotor){
    //copy / move constructor is used here.
    this->LeftMotor = DriveMotor(leftMotor);
    this->RightMotor = DriveMotor(rightMotor);
    this->expectedSpeed = 0.0;
    this->startTime = 0.0;
}

double DriveTrain::getExpectedSpeed(){
    return this->expectedSpeed;
}

void DriveTrain::setExpectedSpeed(double speed){
    this->expectedSpeed = speed;
}

void DriveTrain::ResetPIDVars(){
    this->startTime = TimeNow();
    this->LeftMotor.ResetPIDVars(this->startTime);
    this->RightMotor.ResetPIDVars(this->startTime);
}

void DriveTrain::setPIDConstants(double Pvar, double Ivar, double Dvar){
    this->LeftMotor.setPIDConstants(Pvar,Ivar,Dvar);
    this->RightMotor.setPIDConstants(Pvar,Ivar,Dvar);
}

void DriveTrain::Drive(double distance){
    this->ResetPIDVars();
    while (TimeNow()-startTime <= 0.01);
    while ((LeftMotor.Counts() + RightMotor.Counts())/2 * (1.0/TICKS_PER_INCH) < distance){
        RightMotor.SetPercent(RightMotor.PIDAdjustment(expectedSpeed));
        LeftMotor.SetPercent(LeftMotor.PIDAdjustment(expectedSpeed));
        Sleep(0.05);
    }
    LeftMotor.Stop();
    RightMotor.Stop();
}