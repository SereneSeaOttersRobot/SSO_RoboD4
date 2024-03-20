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
#define LEFT_LF_YLW 0.15
#define RIGHT_LF_YLW 1.65
#define MIDDLE_LF_YLW 0.86
#define TOLERA_LF_YLW 0.1
//name_cds_color value
#define CV_CDS_RED 0.3
#define CV_CDS_BLUE 0.9
#define TOLERA_CDS_COLOR 0.2


//ports
//objecttype_name_port port_value
#define MOTOR_LEFT_PORT FEHMotor::Motor0
#define MOTOR_RIGHT_PORT FEHMotor::Motor2
#define MOTOR_FORKLIFT_PORT FEHMotor::Motor3
#define ENCODER_LEFTS_PORT FEHIO::P3_0
#define ENCODER_LEFTD_PORT FEHIO::P0_1
#define ENCODER_RIGHTS_PORT FEHIO::P0_3
#define ENCODER_RIGHTD_PORT FEHIO::P0_2
#define LIGHTSENSOR_CDS_PORT FEHIO::P0_0
#define LIGHTFOLLOWER_LEFT_PORT FEHIO::P3_2
#define LIGHTFOLLOWER_MIDDLE_PORT FEHIO::P1_2
#define LIGHTFOLLOWER_RIGHT_PORT FEHIO::P1_0
//same needs done for microswitchs
#define BUTTON_FORKTOP_PORT FEHIO::P0_0
#define BUTTON_FORKBOT_PORT FEHIO::P0_0

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

/**
 * @brief Moves the robot by the given number of ticks and at the given percent speed.
 * @param ticks the amount of ticks to move this by, can be negative to move backwards.
 * @param percent the percent SPEED to move this by, not velocity. 
 * @return void, however encoders and motors objects will be updated by end of this.
 *  Encoders will have direction updated then updated again to 'forward' by the end of this.
 *  Robot will stop at the end of this. ticks travel = ticks sent + 1, as it runs while ticks are 'less' than or equal
 *  to the sent ticks. 
 */
void move(int ticks, float percent);

/*
Global objects
*/

Motors motors;
LightSensor lsensor;
Encoders encoders;
AnalogInputPin lf_left(LIGHTFOLLOWER_LEFT_PORT);
AnalogInputPin lf_middle(LIGHTFOLLOWER_MIDDLE_PORT);
AnalogInputPin lf_right(LIGHTFOLLOWER_RIGHT_PORT);
FEHMotor forklift(MOTOR_FORKLIFT_PORT,VOLTAGE_FORKLIFT);

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
    //Encoders
    AnalogEncoder temporay_leftEncoderS(ENCODER_LEFTS_PORT);
    AnalogEncoder temporay_rightEncoderS(ENCODER_RIGHTS_PORT);

    encoders.left = &temporay_leftEncoderS;
    encoders.right = &temporay_rightEncoderS;

    encoders.setThresholds(ENCODER_LOW,ENCODER_HIGH);

    //***Test Code*****
    /*
    //finding line light values
    while (0){
        LCD.Clear();
        LCD.Write("Left: ");
        LCD.WriteLine(lf_left.Value());
        LCD.Write("Middle: ");
        LCD.WriteLine(lf_middle.Value());
        LCD.Write("Right: ");
        LCD.WriteLine(lf_right.Value());
        Sleep(0.2);
    }

    //working the forklift
    while (0){
        //use bummpers to determine direction of forklift motor
        DigitalInputPin bottom(BUTTON_FORKBOT_PORT);
        DigitalInputPin top(BUTTON_FORKTOP_PORT);
        const float percent = 90.0;
        while(1){
            if (!bottom.Value()){
                forklift.SetPercent(-percent);
            } else if (!top.Value()){
                forklift.SetPercent(percent);
            } else {
                forklift.Stop();
            }
        }
    }


    if (0){
        //Test code number
        return 2;
    }
    */
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
        //should moving straight forward, not actually sleeping just move certain number of ticks.
    //move(40, 20.0); //move forward 
    motors.setPerc(20,15);
    Sleep(4.0);
    motors.stop();
    //move until detect line
    bool haveDetectedMiddle = false;
    encoders.setDir(true,true);
    motors.setPerc(15.0,20.0);
    do {
        haveDetectedMiddle = lineEqual(lf_middle.Value(),MIDDLE_LF_YLW,TOLERA_LF_YLW);
        //display to screen current status?
        LCD.Clear();
        LCD.Write("Left: ");
        LCD.WriteLine(lf_left.Value());
        LCD.Write("Middle: ");
        LCD.WriteLine(lf_middle.Value());
        LCD.Write("Right: ");
        LCD.WriteLine(lf_right.Value());
        Sleep(0.2);

    } while (!haveDetectedMiddle);
    //center on line
        //continue while detecting middle, until detect left, then enter linefollower logic with initial state as left.
    bool haveDetectedLeft = false;
    do {
        haveDetectedMiddle = lineEqual(lf_middle.Value(),MIDDLE_LF_YLW,TOLERA_LF_YLW);
        haveDetectedLeft = lineEqual(lf_left.Value(),LEFT_LF_YLW,TOLERA_LF_YLW);
        LCD.Clear();
        LCD.Write("Left: ");
        LCD.WriteLine(lf_left.Value());
        LCD.Write("Middle: ");
        LCD.WriteLine(lf_middle.Value());
        LCD.Write("Right: ");
        LCD.WriteLine(lf_right.Value());
        Sleep(0.2);
    } while (haveDetectedMiddle && !haveDetectedLeft);
    //line follow to head (all detect line, stop here)
        //assuming current state is Middle (nope) and has been set to Middle (left)
    int state = LEFT, stateCount = 0; //if stateCount reaches three...
    do {
        switch(state){
            case MIDDLE:
                {   
                    //display
                    LCD.Clear();
                    LCD.Write("Left: ");
                    LCD.WriteLine(lf_left.Value());
                    LCD.Write("Middle: ");
                    LCD.WriteLine(lf_middle.Value());
                    LCD.Write("Right: ");
                    LCD.WriteLine(lf_right.Value());
                    //intialize
                    stateCount = 0;
                    encoders.setDir(true,true);
                    motors.setPerc(15.0);
                    //logic
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
                    LCD.Clear();
                    LCD.Write("Left: ");
                    LCD.WriteLine(lf_left.Value());
                    LCD.Write("Middle: ");
                    LCD.WriteLine(lf_middle.Value());
                    LCD.Write("Right: ");
                    LCD.WriteLine(lf_right.Value());
                    stateCount = 0;
                    encoders.setDir(false, true);
                    motors.setPerc(-15.0, 15.0);
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
                    LCD.Clear();
                    LCD.Write("Left: ");
                    LCD.WriteLine(lf_left.Value());
                    LCD.Write("Middle: ");
                    LCD.WriteLine(lf_middle.Value());
                    LCD.Write("Right: ");
                    LCD.WriteLine(lf_right.Value());
                    stateCount = 0;
                    encoders.setDir(true, false);
                    motors.setPerc(15.0, -15.0);
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
        Sleep(0.2);
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

void move(int ticks, float percent){
    int leftTicks = 0, rightTicks = 0;
    bool beforeGoal; //true when currentTicks comes before sent Ticks, i.e. -ticks < curTicks or ticks > curticks
    encoders.resetTicks();
    if (ticks < 0){
        encoders.setDir(false, false);
        //expression for negative direction.
        beforeGoal = (leftTicks+rightTicks)/2 >= ticks;
        motors.setPerc(-percent);
    } else {
        encoders.setDir(true, true);
        //expression for positive direction.
        beforeGoal = (leftTicks+rightTicks)/2 <= ticks;
        motors.setPerc(percent);
    }
    do {
        encoders.ticks(leftTicks,rightTicks);
        //display ticks here if needed
        if (ticks<0){
            beforeGoal = (leftTicks+rightTicks)/2 >= ticks;
        } else {
            beforeGoal = (leftTicks+rightTicks)/2 <= ticks;
        }

    } while (beforeGoal);
    motors.stop();

}