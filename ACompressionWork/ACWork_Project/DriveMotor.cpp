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
    float pTerm = error * P;
    float iTerm = errorSum * I;
    float dTerm = (error - this->lastError) * D;
    lastError = error;
    lastTime = timeNow;
    lastCounts = counts;
    return currentPercent+pTerm+iTerm+dTerm;

}

void DriveMotor::resetin(AnalogEncoder &encoder){
    encoder.ResetCounts();
}


DriveTrain::DriveTrain(DriveMotor &leftMotor, DriveMotor &rightMotor): RightEncoder(Null_Port), LeftEncoder(Null_Port){
    //use assignment operator, assign default DriveMotor members to passed by ref DriveMotor objects.
    this->LeftMotor = leftMotor;
    this->RightMotor = rightMotor;
}

void DriveTrain::setEncoders(AnalogEncoder &leftEncoder, AnalogEncoder &rightEncoder){
    this->LeftEncoder = leftEncoder;
    this->RightEncoder = rightEncoder;
}

void DriveTrain::testDriveRight(){
    
    LCD.WriteLine("Starting drive motor test");
    int tickLim = 20;
    this->RightEncoder.ResetCounts();
    int ticks = this->RightEncoder.Counts();
    this->RightMotor.SetPercent(15.0);
    do {
        Sleep(0.2);
        ticks = this->RightEncoder.Counts();
        LCD.Write(ticks);
        LCD.Write("/");
        LCD.WriteLine(tickLim);
    } while (ticks < tickLim);
    LCD.WriteLine("Stopping");
    this->RightMotor.Stop();
}

