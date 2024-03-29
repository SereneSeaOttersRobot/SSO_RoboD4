#ifndef DRIVEMOTOR_H
#define DRIVEMOTOR_H

#include <FEHMotor.h>
#include <FEHIO.h>
#include <Constants.h>

class DriveMotor: private FEHMotor, private AnalogEncoder{

    public :

    /**
     * Creates a DriveMotor object with motor and encoder capibalities.
     * @param motorPort I/O port to interact with physical motor
     * @param voltage voltage level to run motor at.
     * @param encoderPin I/O pin to interact with physical encoder
    */
    DriveMotor(FEHMotorPort motorPort, float voltage, FEHIO::FEHIOPin encoderPin);

    /**
     * Overriden method: Sets Motor percent to the provided agrument. 
     * Requires -100 <= percent <= 100 .
     * @param percent motor percentage
    */
    void SetPercent(float percent);

    /**
     * Overriden method: Stops motor movement.
    */
    void Stop();

    /**
     * Overriden method: Resets the encoder counts.
    */
    void ResetCounts();

    /**
     * Overriden method: Returns the number of counts recorded by the encoder.
     * @return counts recorded since last reset.
    */
    int Counts();

    /**
     * Returns the current motor percent.
     * @return the current motor percentage.
    */
    float CurrentPercent();

    void ResetPIDVars(double startTime);

    void setPIDConstants(double Pvar, double Ivar, double Dvar);

    /**
     * returns an adjusted motor percentage to match expectedSpeed
     * @param expectedSpeed the speed to traverse at, inches/sec
     * @return motor power percentage.
    */
    float PIDAdjustment(double expectedSpeed);

    private :
    float currentPercent;
    int lastCounts;
    double lastTime;
    double sumError;
    double preError;
    double Pconst;
    double Iconst;
    double Dconst;
    
};

class DriveTrain{
    
    public:
    /**
     * Creates a drivetrain object with left and right drive motors.
    */
    DriveTrain(DriveMotor leftMotor, DriveMotor rightMotor);

    double getExpectedSpeed();

    void setExpectedSpeed(double speed);

    void ResetPIDVars();

    void setPIDConstants(double Pvar, double Ivar, double Dvar);

    void Drive(double distance);

    private:
    DriveMotor LeftMotor = DriveMotor(Null_Motor_Port, 0.0, Null_Port);
    DriveMotor RightMotor = DriveMotor(Null_Motor_Port, 0.0, Null_Port);
    double expectedSpeed;
    double startTime;

};

#endif