#ifndef DRIVEMOTOR_H
#define DRIVEMOTOR_H

#include <FEHMotor.h>
#include <FEHIO.h>
#include <Constants.h>



class DriveMotor: public FEHMotor{

    public :

    DriveMotor(FEHMotorPort motorPort, float voltage);

    DriveMotor();

    /***** Overriden funcitons ******/
    /**
     * Ovrriden function of FEHMotor. Sets motor percentage to the argued perc.
     * @param perc float percentage to set motor percentage to. Between -100 and 100
    */
    void SetPercent(float perc);
    /**
     * Overriden function of FEHMotor. Stops the motor movement.
    */
    void Stop();

    void resetin(AnalogEncoder &encoder);

    /******* Functions added for PID ********/

    void resetPIDVars();

    float adjustPID(float expectedSpeed, int counts);

    float currentPerc();

    private:
    //Data members
    float currentPercent;
    float lastTime;
    int lastCounts;
    float lastError;
    float errorSum;
};

class DriveTrain{
    
    public:

    DriveTrain(DriveMotor &leftMotor, DriveMotor &rightMotor);

    /**
     * Reset the PID vars (not contants)
    */
    void resetPIDVars();

    /**
     * Drives the given inch amount at the set speed.
     * @param inches the distance in inches to drive
     * @param leftEncoder the pass-by-ref left encoder object
     * @param rightEncoder the pass-by-ref right encoder object
    */
    void Drive(float inches, AnalogEncoder &leftEncoder, AnalogEncoder &rightEncoder);

    /**
     * Reports the current Speed this is set to.
     * @return float inches/sec
    */
    float currentSpeed();

    /**
     * Sets the speed of this.
     * @param speed the inches/secs speed to set this to.
    */
    void setSpeed(float speed);

    private:
    DriveMotor LeftMotor = DriveMotor();
    DriveMotor RightMotor = DriveMotor();
    float expSpeed;

};

#endif