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

    void ResetCounts();

    int Counts();

    float CurrentPercent();

    

    
};

#endif