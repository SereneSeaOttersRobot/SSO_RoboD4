#ifndef DRIVEMOTOR_H
#define DRIVEMOTOR_H

#include <FEHMotor.h>
#include <FEHIO.h>
#include <Constants.h>



class DriveMotor: public FEHMotor{

    public :

    DriveMotor(FEHMotorPort motorPort, float voltage);

    DriveMotor();

    friend class DriveTrain;

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

    /**
     * Test function for determining level of manipulation for encoder objects.
     * Resets the counts for the given encoder.
    */
    void resetin(AnalogEncoder &encoder);

    /**
     * Reports the current percent this is set to.
     * @return current motor percentage.
    */
    float currentPerc();

    private:
    /******* Functions added for PID ********/

    /**
     * Resets the related PID variables
    */
    void resetPIDVars();

    /**
     * Returns an adjusted motor percentage based on the given inputs, other information, and current motor percentage.
     * @param expectedSpeed expected velocity for this to travel at.
     * @param counts the counts from the associated encoder
     * @return percentage for motor
    */
    float adjustPID(float expectedSpeed, int counts);

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
     * Turns this based on the given degrees, left and right is determined by the setSpeed.
     * Negative turns left, while Positive turns right.
     * @param degrees the amount of degrees to turn.
     * @param leftEncoder pass-by-ref encoder object for the left encoder
     * @param rightEncoder pass-by-ref encoder object for the right encoder
    */
    void Turn(float degrees, AnalogEncoder &leftEncoder, AnalogEncoder &rightEncoder);

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