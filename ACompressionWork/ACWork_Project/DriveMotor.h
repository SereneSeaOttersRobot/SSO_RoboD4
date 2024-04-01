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

    void setEncoders(AnalogEncoder &leftEncoder, AnalogEncoder &rightEncoder);

    void testDriveRight();

    

    private:
    DriveMotor LeftMotor = DriveMotor();
    DriveMotor RightMotor = DriveMotor();
    AnalogEncoder LeftEncoder;
    AnalogEncoder RightEncoder;

};

#endif