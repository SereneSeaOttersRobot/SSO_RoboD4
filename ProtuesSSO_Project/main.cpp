#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>


/*
Function prototypes and contracts
*/

/**
 * Turns the robot @param degrees and reports the turned amount by the end of the method.
 * @endif Ends if turned = @param degrees or @param timeLimit reached or @param sensor_bumper_* hit.
 * @returns amount turned in degrees
 * @param degrees - The amount of degrees to turn
 * @param timeLimit - The time limit for this run time, if this runtime exceeds timeLimit, returns early
 * @param motor_*
 * @param sensor_bumper_*
 * @param sensor_encoder_*
 * @updates motor_*, sensor_bumper_*, sensor_encoder_*
 * @pre none
 * @post ensures turn = [amount turned] iff ( @param degrees = [amount turned] or [runtime] exceeds @param timeLimit) 
 * or turn = -1 * [amount turned] iff ( @param sensor_bumper_*.Value() = false), and -360 < turn < 360 
 */
float turn(float degrees, int timeLimit);

/**
 * Moves robot forward by @param inches and reports the amount moved by the end of the method.
 * @endif Ends if [amount moved] = @param inches or [runtime of this] exceeds @param timeLimit or @param sensor_bumper_* hit
 * @returns amount moved in inches
 * @param inches - the amount of inches to move
 * @param timeLimit - the time limit for this run time, if this runtime exceeds timeLimit, returns early
 * @param motor_*
 * @param sensor_bumper_*
 * @param sensor_encoder_*
 * @updates motor_*, sensor_bumper_*, sensor_encoder_*
 * @pre none
 * @post ensures move = [amount moved] iff ( @param inches = [amount moved] or [runtime] exceeds @param timeLimit) 
 * or move = -1 * [amount moved] iff ( @param sensor_bumper_*.Value() = false)
 */
float move(float inches, int timeLimit);

bool withinStartLightRange(float lightValue);

bool withinTicketLightRange(float lightValue);

/*
Global declarations of hardware objects
*/
//Motors
const static float motor_voltage = 1.0;
FEHMotor motor_left(FEHMotor::Motor0, motor_voltage);
FEHMotor motor_right(FEHMotor::Motor1, motor_voltage);
//*************** Sensors ***************
//******analog*******
//light color sensor (cds)
const static float sensor_cds_voltage = 3.3;
AnalogInputPin sensor_cds(FEHIO::P0_0);

//******digital*******
DigitalInputPin sensor_bumper_pos1(FEHIO::P0_1);
DigitalInputPin sensor_bumper_pos2(FEHIO::P0_2);
DigitalInputPin sensor_bumper_pos3(FEHIO::P0_3);
//...


/*
Global declarations for non-hardware variables.
*/

//start light
float start_light = 0;

int main(void)
{   
    //start
    LCD.Clear(BLACK);
    LCD.WriteLine("Touch to ready up");
    //Wait on touch
    float *x, *y;
    while(!LCD.Touch(x,y));
    while(LCD.Touch(x,y));

    //wait for acceptable value from light sensor
    int updateWaitTicks = 10, ticks = 10;
    float lightValue;
    do{
        lightValue = sensor_cds.Value();
        if (ticks == updateWaitTicks){
            LCD.Clear(BLACK);
            LCD.WriteLine("detected light value");
            LCD.WriteLine(lightValue);
            ticks = 0;
        }
        ticks++;
    }while(!withinStartLightRange(lightValue));
    LCD.Clear(BLACK);
    LCD.WriteLine("detected and saved light value");
    LCD.WriteLine(lightValue);
    start_light = lightValue;

    /*
    Main function constants
    */
    const int time_limit = 2; // 2 second time limit base
    //********

    //**ISSUE** floater math is not exact, == or != is bad when comparing floats, high chance of error

    //Turn and move to ramp
    float turn1 = 45.0;
    float turned1 = turn(turn1, time_limit);
    if (turn1 != turned1){
        //error handling
    }
    float move1 = 12.0; //inches
    float moved1 = move(move1, time_limit);
    if (move1 != moved1){
        //error handling
    }



}

float turn(float degrees, int timeLimit){
    //here for compiler error handling, replace with actual implementation.
    float res = 0.0;
    //remove full revolutions from turn.
    int numRevInt = ((int)degrees)/360;
    const float desired_degrees = degrees - (360*numRevInt);

    return res;
}

float move(float inches, int timeLimit){
    //here for compiler error handling, replace with actual implementation.
    float res = 0.0;

    return res;
}

bool withinStartLightRange(float lightValue){
    //here for compiler error handling, replace with actual implementation.
    bool res = false;

    return res;
}

bool withinTicketLightRange(float lightValue){
    //here for compiler error handling, replace with actual implementation.
    bool res = false;

    return res;
}
