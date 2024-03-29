#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>
#include <FEHBattery.h>
#include <iostream>
#include <thread>

using namespace std;

/*
hashtag defines
*/
// Button pressed
#define BP 0
// Button not pressed
#define BNP 1

// color values
// name_linefollower_color value
#define LEFT_LF_YLW 0.15    //Left_LineFollower_Yellow
#define RIGHT_LF_YLW 1.65   //Right_LineFollower_Yellow
#define MIDDLE_LF_YLW 0.86  //Middle_LineFollower_Yellow
#define TOLERA_LF_YLW 0.1   //Tolerance_LineFollower_Yellow
#define LEFT_LF_WH 0.159    //Left_LineFollower_White
#define RIGHT_LF_WH 2.00    //Right_LineFollower_White
#define MIDDLE_LF_WH 1.25   //Middle_LineFollower_White
#define TOLERA_LF_Wh 0.15   //Tolerance_LineFollower_White
// name_cds_color value
#define CV_CDS_RED 0.3          //ColorValue_CdS_Red  
#define CV_CDS_BLUE 0.9         //ColorValue_CdS_Blue
#define TOLERA_CDS_COLOR 0.2    //Tolerance_CdS_Color

// 12.73239545 ticks/in
#define TICKS_PER_INCH 12.732

// 0.8888889222 ticks/degree
#define TICKS_PER_DEGREE 0.88888


// ports
// objecttype_name_port port_value
#define MOTOR_LEFT_PORT FEHMotor::Motor0
#define MOTOR_RIGHT_PORT FEHMotor::Motor2
#define MOTOR_FORKLIFT_PORT FEHMotor::Motor3
#define ENCODER_LEFTS_PORT FEHIO::P3_0      //Single Signal Left Encoder Port
#define ENCODER_LEFTD_PORT FEHIO::P0_1      //Double Signal Left Encoder Port
#define ENCODER_RIGHTS_PORT FEHIO::P0_3     //Single Signal Right Encoder Port
#define ENCODER_RIGHTD_PORT FEHIO::P0_2     //Double Signal Right Encoder Port
#define LIGHTSENSOR_CDS_PORT FEHIO::P0_0
#define LINEFOLLOWER_LEFT_PORT FEHIO::P3_2
#define LINEFOLLOWER_MIDDLE_PORT FEHIO::P1_2
#define LINEFOLLOWER_RIGHT_PORT FEHIO::P1_0
// same needs done for microswitchs
#define BUTTON_FORKTOP_PORT FEHIO::P0_7
#define BUTTON_FORKBOT_PORT FEHIO::P3_4
#define BUTTON_FORKLIFT_PORT FEHIO::P3_5

// Voltages
// voltage_name values
#define VOLTAGE_MOTORS 9.0
#define VOLTAGE_FORKLIFT 5.0

// Encoder thresholds
#define ENCODER_LOW 0.15
#define ENCODER_HIGH 2.35

/*
Enumerations
*/
enum LineStates
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2

};

/*
Function prototypes
*/

/**
 * @brief determines if x is equal to y within linefollower tolerance toler
 * @param x linefollower returned value
 * @param y linefollower acceptable value
 * @param toler linefollower tolerance (+-)
 * @return true if x is with y+toler and y-toler
 */
bool lineEqual(float x, float y, float toler);

/**
 * @brief turns robot right by the given degrees at given percent speed.
 * 
 * Uses global encoder and motor objects
 * 
 * @param degree number of degrees to turn
 * @param percent the percent speed to turn, between 100 and >0 
 */
void turnRight(double degree, float percent);
/**
 * @brief turns robot left by the given degrees at given percent speed
 * 
 * Uses global encoder and motor objects
 * 
 * @param degree number of degrees to turn
 * @param percent the percent speed to turn, between 100 and >0
 */
void turnLeft(double degree, float percent);
/**
 * @brief move forward until a displacement of the given inches and with a speed percent.
 * 
 * Uses global encoder and motor objects
 * 
 * @param inches the number of inches to displace the robot forward by. inches>0
 * @param percent the percent speed to move the robot by. 100>=percent>0
 */
void moveForward(double inches, float percent);
/**
 * @brief move backward until a displacement of the given inches and with a speed percent.
 * 
 * Uses global encoder and motor objects
 * 
 * @param inches the number of inches to displace the robot forward by. inches>0
 * @param percent the percent speed to move the robot by. 100>=percent>0
 */
void moveBackward(double inches, float percent);

/**
 * @brief Smartly move straight forward or backward.
 * While moving, this actively tries to maintain a straight course.
 * Forward or backward movement is determine by percent signage
 * 
 * Uses global encoder and motor objects.
 * 
 * @param inches the amount of distance to travel in a straight line. inches>0
 * @param percent the percent velocity to move. Determine direction. 100 >= percent >= -100  
 */
void move(double inches, float percent);

//Test function, moves forklift to bottom state
void forkToBottom(FEHMotor forklif, DigitalInputPin forkbot);

/*
Global objects
*/

FEHMotor leftMotor(MOTOR_LEFT_PORT, VOLTAGE_MOTORS);    //Left Motor object
FEHMotor rightMotor(MOTOR_RIGHT_PORT, VOLTAGE_MOTORS);  //Right Motor object
AnalogInputPin sensor_cds(LIGHTSENSOR_CDS_PORT);        //CdS light sensor object
AnalogEncoder leftEncoder(ENCODER_LEFTS_PORT);          //one signal encoder object for left motor
AnalogEncoder rightEncoder(ENCODER_RIGHTS_PORT);        //one signal encoder object for right motor
AnalogInputPin lf_left(LINEFOLLOWER_LEFT_PORT);         //left line follower object
AnalogInputPin lf_middle(LINEFOLLOWER_MIDDLE_PORT);     //middle line follower object
AnalogInputPin lf_right(LINEFOLLOWER_RIGHT_PORT);       //right line follower object
FEHMotor forklift(MOTOR_FORKLIFT_PORT, VOLTAGE_FORKLIFT); //Forklift servo/motor object
DigitalInputPin forkbottom(BUTTON_FORKBOT_PORT);        //micro switch button object at bottom of forklift
DigitalInputPin forktop(BUTTON_FORKTOP_PORT);           //micro switch button object at top of forklift
DigitalInputPin forkbumper(BUTTON_FORKLIFT_PORT);       //micro switch button on the forklift


/**
 * 
 */
int main()
{   
    LCD.Clear();
    LCD.WriteLine("Starting Test");
    //Setting thresholds for encoder objects and resetting their counts to 0
    rightEncoder.SetThresholds(0.15, 2.35);
    rightEncoder.ResetCounts();
    leftEncoder.SetThresholds(0.15, 2.35);
    leftEncoder.ResetCounts();

    //hardcoded upper and lower bounds of red light values
    float x = 0.5;
    float y = 0.1;
    int light = 0;
    int lightColor = 0;

    std::thread fork(forkToBottom, forklift, forkbottom);
    moveForward(5.0, 15.0);
    fork.join();


    return 1;
}

void forkToBottom(FEHMotor forklif, DigitalInputPin forkbot){
    int bp = 0;
    float perc = -90.0;
    forklif.SetPercent(perc);
    while (forkbot.Value() != bp);
    forklif.Stop();
}

bool lineEqual(float x, float y, float toler)
{
    bool res = false;
    if (y - toler <= x && x <= y + toler)
    {
        res = true;
    }
    return res;
}

void moveForward(double inches, float percent)
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
    int ticks = (int)(inches * TICKS_PER_INCH / 1.22);
    leftMotor.SetPercent(percent + 0.75);
    rightMotor.SetPercent(percent);
    while (((leftEncoder.Counts() + rightEncoder.Counts()))/2  < ticks)
        ;
    leftMotor.Stop();
    rightMotor.Stop();
    

}

void moveBackward(double inches, float percent)
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
    int ticks = (int)(inches * TICKS_PER_INCH / 1.22);
    leftMotor.SetPercent(-percent - 1.4);
    rightMotor.SetPercent(-percent + 0.5);
    while ((leftEncoder.Counts() + rightEncoder.Counts()) / 2 < ticks)
        ;
    leftMotor.Stop();
    rightMotor.Stop();
}

void turnLeft(double degree, float percent)
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
    int ticks = (int)(degree * TICKS_PER_DEGREE);
    leftMotor.SetPercent(-percent);
    rightMotor.SetPercent(percent);
    while ((rightEncoder.Counts() + leftEncoder.Counts() / 2) < ticks)
        ;
    rightMotor.Stop();
    leftMotor.Stop();
}

void turnRight(double degree, float percent)
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
    int ticks = (int)(degree * TICKS_PER_DEGREE);
    leftMotor.SetPercent(percent);
    rightMotor.SetPercent(-percent);
    while ((leftEncoder.Counts() + rightEncoder.Counts() / 2) < ticks)
        ;
    rightMotor.Stop();
    leftMotor.Stop();
}

void move(double inches, float percent){
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
    int dir = 1;
    if (percent < 0){
        dir = -1;
    }
    int ticks = (int)(inches * TICKS_PER_INCH / 1.22);
    int modifier = 1.0;
    leftMotor.SetPercent(percent);
    rightMotor.SetPercent(percent);
    int l, r;
    while ((rightEncoder.Counts() < (ticks/2)) && (leftEncoder.Counts() < (ticks)/2)){
        l = leftEncoder.Counts();
        r = rightEncoder.Counts();
        //idea if shaft encoders update on different times, then can error tolerate with...
        //r == l being r==l || r-1==l || r==l-1
        if (r == l){
            leftMotor.SetPercent(percent);
            rightMotor.SetPercent(percent);
        } else if (l<r) {
            leftMotor.SetPercent(percent + dir*modifier);
            rightMotor.SetPercent(percent - dir*modifier);
        } else if (r<l){
            rightMotor.SetPercent(percent + dir*modifier);
            leftMotor.SetPercent(percent - dir*modifier);
        }
    }
    leftMotor.Stop();
    rightMotor.Stop();
}