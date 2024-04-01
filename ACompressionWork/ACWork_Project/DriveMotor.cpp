#include <DriveMotor.h>
#include <Constants.h>
#include <FEHUtility.h>
#include <FEHLCD.h>

DriveMotor::DriveMotor(FEHMotorPort motorPort, float voltage): FEHMotor(motorPort,voltage){
    this->currentPercent = 0.0;
}

DriveMotor::DriveMotor(): FEHMotor(Null_Motor_Port, 0.0){}


void  DriveMotor::SetPercent(float percent){
    this->currentPercent = percent;
    FEHMotor::SetPercent(percent);
}

void DriveMotor::Stop(){
    FEHMotor::Stop();
    this->currentPercent = 0.0;
}

void DriveMotor::resetPIDVars(){
    this->errorSum = 0.0;
    this->lastCounts = 0;
    this->lastError = 0.0;
    this->lastTime = TimeNow();
}

float DriveMotor::adjustPID(float expectedSpeed, int counts){
    float timeNow = TimeNow();
    float actualSpeed = INCHES_PER_TICK * ((counts - this->lastCounts) / (timeNow - lastTime));
    float error = expectedSpeed - actualSpeed;
    this->errorSum += error;
    float pTerm = error * PID.P;
    float iTerm = errorSum * PID.I;
    float dTerm = (error - this->lastError) * PID.D;
    lastError = error;
    lastTime = timeNow;
    lastCounts = counts;
    return currentPercent+pTerm+iTerm+dTerm;

}

void DriveMotor::resetin(AnalogEncoder &encoder){
    encoder.ResetCounts();
}


DriveTrain::DriveTrain(DriveMotor &leftMotor, DriveMotor &rightMotor){
    //use assignment operator, assign default DriveMotor members to passed by ref DriveMotor objects.
    this->LeftMotor = leftMotor;
    this->RightMotor = rightMotor;
    this->expSpeed = 0.0;
}


void DriveTrain::resetPIDVars(){
    this->RightMotor.resetPIDVars();
    this->LeftMotor.resetPIDVars();
}


void DriveTrain::Drive(float inches, AnalogEncoder &leftEncoder, AnalogEncoder &rightEncoder){
    this->resetPIDVars();
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
    const int ticks = inches*TICKS_PER_INCH;
    int left, right;
    this->RightMotor.SetPercent(15.0);
    this->LeftMotor.SetPercent(15.0);
    do {
        Sleep(0.2);
        left = leftEncoder.Counts();
        right = rightEncoder.Counts();
        this->RightMotor.SetPercent(this->RightMotor.adjustPID(this->expSpeed, right));
        this->LeftMotor.SetPercent(this->LeftMotor.adjustPID(this->expSpeed, left));
    } while ((left + right)/2 < ticks);
    this->LeftMotor.Stop();
    this->RightMotor.Stop();
}


float DriveTrain::currentSpeed(){
    return this->expSpeed;
}


void DriveTrain::setSpeed(float speed){
    this->expSpeed = speed;
}



