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
#define MOTOR_VOLT 9.0

//hardware global declarations

FEHMotor tempLeft(FEHMotor::Motor0, MOTOR_VOLT);
FEHMotor tempRight(FEHMotor::Motor2, MOTOR_VOLT);


//motors
class Motor {
    public:
    FEHMotor left = tempLeft;
    FEHMotor right = tempRight;
    Motor();
    Motor(){};
    void setPerc(float percent){
        left.SetPercent(percent);
        right.SetPercent(percent);
    }
    void setPerc(float leftP, float rightP){
        left.SetPercent(leftP);
        right.SetPercent(rightP);
    }

    void stop(){
        left.Stop();
        right.Stop();
    }
};

Motor motor;



//cds sensor
AnalogInputPin sensor_cds(FEHIO::P0_4);

//bumper sensor
DigitalInputPin sensor_bumper_left(FEHIO::P3_4);
DigitalInputPin sensor_bumper_front(FEHIO::P0_2);
DigitalInputPin sensor_bumper_right(FEHIO::P0_3);


/**
 * Main function of program
 */
int main(void) {
    
}

bool startLightWithinRange(float lightValue){
    bool res = true; // for testing, change back to false after testing
    //chain of ifs, account for tolerances
    return res;
}
