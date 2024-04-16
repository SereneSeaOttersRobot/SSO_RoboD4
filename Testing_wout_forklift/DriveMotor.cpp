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
    //reset PIDvars and encoder counts
    this->resetPIDVars();
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
    //find needed travel distance as integer ticks instead
    const int ticks = inches*TICKS_PER_INCH;
    //create variables to hold onto encoder counts
    int left, right;
    //set initial motor percent based on 5*expSpeed
    this->RightMotor.SetPercent(7.0*expSpeed);
    this->LeftMotor.SetPercent(7.0*expSpeed);
    //set signage for counts sent through adjustPID
    int dir = -1 + 2*(expSpeed>0.0); //use boolean to integer for -1 vs 1
    do {
        //sleep for time discretion in calculations
        Sleep(0.2);
        //get counts
        left = leftEncoder.Counts();
        right = rightEncoder.Counts();
        //set percentages based on PID returns
        this->RightMotor.SetPercent(this->RightMotor.adjustPID(this->expSpeed, dir*right));
        this->LeftMotor.SetPercent(this->LeftMotor.adjustPID(this->expSpeed, dir*left));
    } while ((left + right)/2 < ticks);
    this->LeftMotor.Stop();
    this->RightMotor.Stop();
}

void DriveTrain::Turn(float degrees, AnalogEncoder &leftEncoder, AnalogEncoder &rightEncoder){
    //reset PIDVars and encoder counts
    this->resetPIDVars();
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
    //find needed rotation distance as integer ticks insteads
    const int ticks = degrees*TICKS_PER_DEGREE;
    //create vars to hold counts
    int left, right;
    //set signage based on expectedSpeed, dir * left, -dir*right
    int dir = 1 - 2*(expSpeed<0.0);
    //set initial motor percents based on expSpeed
    this->LeftMotor.SetPercent(7.0*expSpeed);
    this->RightMotor.SetPercent(7.0*-expSpeed);
    do {
        //sleep for time discretion in calculations
        Sleep(0.2);
        //get counts from encoders
        left = leftEncoder.Counts();
        right = rightEncoder.Counts();
        //set percentages based on PID returns
        this->LeftMotor.SetPercent(this->LeftMotor.adjustPID(expSpeed, dir*left));
        this->RightMotor.SetPercent(this->RightMotor.adjustPID(-expSpeed, -dir*right));
    } while ((left+right)/2 < ticks && LeftMotor.currentPercent < 30.0);
    this->LeftMotor.Stop();
    this->RightMotor.Stop();
    //tries to handle momentum overshoot by recalling itself for the opposite direction by the offshoot amount
    // Sleep(0.2);
    // int offTicks = (left+right)/2 - ticks;
    // if (offTicks > 2){
    //     expSpeed = -expSpeed/2.0;
    //     this->Turn(offTicks*DEGREES_PER_TICK, leftEncoder, rightEncoder);
    // }

}


float DriveTrain::currentSpeed(){
    return this->expSpeed;
}


void DriveTrain::setSpeed(float speed){
    this->expSpeed = speed;
}



