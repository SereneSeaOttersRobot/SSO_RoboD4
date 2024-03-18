#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>
#include <Motors.h>
#include <LightSensor.h>
#include <Encoders.h>

/*
hashtag defines
*/
//color values
//name_linefollower_color value
#define LEFT_LF_YLW 0.00
#define RIGHT_LF_YLW 0.00
#define MIDDLE_LF_YLW 0.00
#define TOLERA_LF_YLW 0.1
//name_cds_color value
#define CV_CDS_RED 0.3
#define CV_CDS_BLUE 0.9
#define TOLERA_CDS_COLOR 0.2


//ports
//objecttype_name_port port_value
#define MOTOR_LEFT_PORT FEHMotor::Motor0
#define MOTOR_RIGHT_PORT FEHMotor::Motor1
#define MOTOR_FORKLIFT_PORT FEHMotor::Motor2
#define ENCODER_LEFTS_PORT FEHIO::P0_0
#define ENCODER_LEFTD_PORT FEHIO::P0_1
#define ENCODER_RIGHTS_PORT FEHIO::P0_2
#define ENCODER_RIGHTD_PORT FEHIO::P0_3
#define LIGHTSENSOR_CDS_PORT FEHIO::P0_4
#define LIGHTFOLLOWER_LEFT_PORT FEHIO::P0_5
#define LIGHTFOLLOWER_RIGHT_PORT FEHIO::P0_6
#define LIGHTFOLLOWER_MIDDLE_PORT FEHIO::P0_7
//same needs done for microswitchs

//Voltages
//voltage_name values
#define VOLTAGE_MOTORS 9.0
#define VOLTAGE_FORKLIFT 5.0

//Encoder thresholds
#define ENCODER_LOW 0.15
#define ENCODER_HIGH 2.35

/*
Enumerations
*/
enum LineStates {
    LEFT = -1,
    MIDDLE = 0,
    RIGHT = 1

};

/*
Function prototypes
*/

/**
 * @brief determines if x is equal to y within linefollower tolerance toler
 * @param x linefollower returned value
 * @param y linefollower acceptable value
 * @param toler linefollower tolerance (+-)
 * @return true if x is with toler of y.
 */
bool lineEqual(float x, float y, float toler);

/*
Global objects
*/

Motors motors;
LightSensor lsensor;
Encoders encoders;
AnalogInputPin lf_left(LIGHTFOLLOWER_LEFT_PORT);
AnalogInputPin lf_middle(LIGHTFOLLOWER_MIDDLE_PORT);
AnalogInputPin lf_right(LIGHTFOLLOWER_RIGHT_PORT);

int main(){
    /*
    Complete setup for global objects and other stuff
    */
    //Motors
    FEHMotor temporay_leftMotor(MOTOR_LEFT_PORT,VOLTAGE_MOTORS);
    FEHMotor temporay_rightMotor(MOTOR_RIGHT_PORT,VOLTAGE_MOTORS);
    motors.left = &temporay_leftMotor;
    motors.right = &temporay_rightMotor;
    motors.setMotorDirection(true,true);
    //LightSensor
    AnalogInputPin temporay_cds(LIGHTSENSOR_CDS_PORT);
    lsensor.cds_sensor = &temporay_cds;
    lsensor.setLights(CV_CDS_RED,CV_CDS_BLUE);
    lsensor.setTolerance(TOLERA_CDS_COLOR);


    //****Main algorithm*****

    //wait on red
    LCD.Clear();
    LCD.WriteLine("Waiting on red");
    do {
        LCD.Clear();
        LCD.Write("Detected: ");
        LCD.WriteLine(lsensor.lightValue());
        LCD.WriteLine("Waiting on Red");
        Sleep(0.2);
    } while (!lsensor.detectRed());
    LCD.Clear();
    LCD.Write("Detected: ");
    LCD.WriteLine(lsensor.lightValue());
    LCD.WriteLine("Found Red");
    Sleep(2.0);
    //move sleep
    //move until detect line
    //center on line
    //line follow to head (all detect line, stop here)
        //assuming current state is Middle and has been set to Middle
    int state = MIDDLE, stateCount = 0; //if stateCount reaches three...
    do {
        switch(state){
            case MIDDLE:
                {
                    stateCount = 0;
                    motors.setPerc(15.0);
                    if (lineEqual(lf_left.Value(),LEFT_LF_YLW,TOLERA_LF_YLW)){
                        state = LEFT;
                        stateCount++;
                    }
                    if (lineEqual(lf_right.Value(),RIGHT_LF_YLW,TOLERA_LF_YLW)){
                        state = RIGHT;
                        stateCount++;
                    }
                    if (stateCount == 2 && lineEqual(lf_middle.Value(),MIDDLE_LF_YLW,TOLERA_LF_YLW)){
                        stateCount = 3;
                    }
                    break;
                }
            case LEFT:
                {
                    stateCount = 0;
                    motors.setPerc(4.0, 15.0);
                    if (lineEqual(lf_middle.Value(),MIDDLE_LF_YLW,TOLERA_LF_YLW)){
                        state = MIDDLE;
                        stateCount++;
                    }
                    if (lineEqual(lf_right.Value(),RIGHT_LF_YLW,TOLERA_LF_YLW)){
                        state = RIGHT;
                        stateCount++;
                    }
                    if (stateCount == 2 && lineEqual(lf_left.Value(),LEFT_LF_YLW,TOLERA_LF_YLW)){
                        stateCount = 3;
                    }
                    break;
                }
            case RIGHT:
                {
                    stateCount = 0;
                    motors.setPerc(15.0, 4.0);
                    if (lineEqual(lf_left.Value(),LEFT_LF_YLW,TOLERA_LF_YLW)){
                        state = LEFT;
                        stateCount++;
                    }
                    if (lineEqual(lf_middle.Value(),MIDDLE_LF_YLW,TOLERA_LF_YLW)){
                        state = MIDDLE;
                        stateCount++;
                    }
                    if (stateCount == 2 && lineEqual(lf_right.Value(),RIGHT_LF_YLW,TOLERA_LF_YLW)){
                        stateCount = 3;
                    }
                    break;
                }
        }
    } while (stateCount < 3);
    motors.stop();
    //further logic



}

bool lineEqual(float x, float y, float toler){
    bool res = false;
    if (y-toler <= x && x <= y+toler){
        res = true;
    }
    return res;
}