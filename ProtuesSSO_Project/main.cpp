#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>

//function prototypes
bool startLightWithinRange(float lightValue);

//global variables and defines
//Enumeration of Left, Right,...
#define RIGHT 2
#define LEFT 0
#define FRONT 1

//Motor voltage
#define MOTOR_VOLT 1.0

//hardware global declarations
//motors
FEHMotor motor_left(FEHMotor::Motor0, MOTOR_VOLT);
FEHMotor motor_right(FEHMotor::Motor2, MOTOR_VOLT);

//cds sensor
AnalogInputPin sensor_cds(FEHIO::P0_1);

//bumper sensor
DigitalInputPin sensor_bumper_left(FEHIO::P3_4);
DigitalInputPin sensor_bumper_front(FEHIO::P0_2);
DigitalInputPin sensor_bumper_right(FEHIO::P0_3);


/**
 * Main function of program
 */
int main(void) {
    /*
    Pre load variables
    */
    //ready
    LCD.Clear(BLACK);
    LCD.WriteLine("Touch to ready up");
    //wait touch
    float *x, *y;
    while(!LCD.Touch(x,y)) {
        LCD.ClearBuffer();
    }
    while(LCD.Touch(x,y)){
        LCD.ClearBuffer();
    }
    LCD.WriteLine("GO");

    //light detection wait block
    float lightValue = 0.0;
    bool isAcceptable = false;
    do {
        LCD.WriteLine("In light loop");
        //lightValue = sensor_cds.Value();
        isAcceptable = startLightWithinRange(lightValue);
    } while (!isAcceptable);
    LCD.WriteLine("Out of light loop, at intital motor part");

    motor_left.SetPercent(25.0);
    motor_right.SetPercent(-5.0);
    Sleep(6.0);

    motor_left.Stop();
    motor_right.Stop();

    const int numBum = 3;
    bool bumpers[numBum] = {false, false, false};
    do {
        motor_left.SetPercent(25.0);
        //drift clockwise
        motor_right.SetPercent(20.0);
        do {
            LCD.WriteLine("Bumper sensor group get sensor values");
            bumpers[LEFT] = !sensor_bumper_left.Value();
            //bumpers[RIGHT] = !sensor_bumper_right.Value();
            //bumpers[FRONT] = !sensor_bumper_front.Value();
        } while (!(bumpers[FRONT] || bumpers[LEFT] || bumpers[RIGHT]));
        motor_left.Stop();
        motor_right.Stop();
        LCD.WriteLine("got out of sensor group");
        if (bumpers[FRONT] && bumpers[LEFT] && bumpers[RIGHT]){
            //kill
            return -1;
        } else if (bumpers[FRONT] && bumpers[LEFT]){
            //handle
            motor_left.SetPercent(-25.0);
            motor_right.SetPercent(-25.0);
            Sleep(2.0);
            motor_left.SetPercent(-12.0);
        } else if (bumpers[FRONT] && bumpers[RIGHT]){
            //here
        } else if (bumpers[RIGHT]){
            //here
            motor_left.SetPercent(-25.0);
            motor_right.SetPercent(-12.0);
            Sleep(1.0);
        } else if (bumpers[FRONT]){
            //here
        } else if (bumpers[LEFT] && bumpers[RIGHT]){
            //kill
            return -1;
        } else if (bumpers[LEFT]){
            //handle
            LCD.Clear(BLACK);
            LCD.WriteLine("ahahahahahhahah");
            Sleep(6.0);
            return 1;
        }
        //no need to reset bumper[], do while does that.
    } while(!LCD.Touch(x,y));
    LCD.WriteLine("Wider loop ends with a touch, a touch was detected");
    
}

bool startLightWithinRange(float lightValue){
    bool res = true; // for testing, change back to false after testing
    //chain of ifs, account for tolerances
    return res;
}
