#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>

/*
hashtag defines
*/
// Button pressed
#define BP 0
// Button not pressed
#define BNP 1

// color values
// name_linefollower_color value
#define LEFT_LF_YLW 0.15
#define RIGHT_LF_YLW 1.65
#define MIDDLE_LF_YLW 0.86
#define TOLERA_LF_YLW 0.1
#define LEFT_LF_WH 0.159
#define RIGHT_LF_WH 2.00
#define MIDDLE_LF_WH 1.25
#define TOLERA_LF_Wh 0.15
// name_cds_color value
#define CV_CDS_RED 0.3
#define CV_CDS_BLUE 0.9
#define TOLERA_CDS_COLOR 0.2

#define TICKS_PER_INCH 12.732
// 12.73239545 ticks/in

#define TICKS_PER_DEGREE 0.88888
// 0.8888889222 ticks/degree

// ports
// objecttype_name_port port_value
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
// same needs done for microswitchs
#define BUTTON_FORKTOP_PORT FEHIO::P0_7
#define BUTTON_FORKBOT_PORT FEHIO::P3_5

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
 * @return true if x is with toler of y.
 */
bool lineEqual(float x, float y, float toler);

void turnRight(double degree, float percent);
void turnLeft(double degree, float percent);
/**
 * @brief move forward or backwards by the given inches, velocity is determine by sent percent.
 */
void moveForward(double inches, float percent);
void moveBackward(double inches, float percent);

//smart move forward
void move(double inches, float percent);

/*
Global objects
*/

FEHMotor leftMotor(MOTOR_LEFT_PORT, VOLTAGE_MOTORS);
FEHMotor rightMotor(MOTOR_RIGHT_PORT, VOLTAGE_MOTORS);
AnalogInputPin sensor_cds(LIGHTSENSOR_CDS_PORT);
AnalogEncoder leftEncoder(ENCODER_LEFTS_PORT);
AnalogEncoder rightEncoder(ENCODER_RIGHTS_PORT);
AnalogInputPin lf_left(LIGHTFOLLOWER_LEFT_PORT);
AnalogInputPin lf_middle(LIGHTFOLLOWER_MIDDLE_PORT);
AnalogInputPin lf_right(LIGHTFOLLOWER_RIGHT_PORT);
FEHMotor forklift(MOTOR_FORKLIFT_PORT, VOLTAGE_FORKLIFT);
DigitalInputPin forkbottom(BUTTON_FORKBOT_PORT);
DigitalInputPin forktop(BUTTON_FORKTOP_PORT);

int main()
{
    FEHFile *fout = SD.FOpen("PERFTR.txt","w");

    LCD.Clear();
    LCD.WriteLine("Starting Test");
    rightEncoder.SetThresholds(0.15, 2.35);
    rightEncoder.ResetCounts();
    leftEncoder.SetThresholds(0.15, 2.35);
    leftEncoder.ResetCounts();

    float x = 0.5;
    float y = 0.1;
    int light = 0;
    int lightColor = 0;

    //Test code
    //move forward 10inches and stop
    if (0){
        moveForward(10.0,15.0);
        return 1;
    }

    ///////////////// Wait for red //////////////////////////////
    float lightValue = sensor_cds.Value();

    while (lightValue > x || lightValue < y)
    {
        lightValue = sensor_cds.Value();
        LCD.WriteLine(lightValue);
        Sleep(0.1);
    }
    LCD.Clear();

    //************** Get forklift to bottom state *********************
    float percent = -90.0;
    forklift.SetPercent(percent);
    if (forktop.Value() == BP){
        Sleep(0.2);
    }
    while (forkbottom.Value() == BNP)
    {
        // if forklift is going up with percent, then top hit, switching direction of motor.
        if (forktop.Value() == BP)
        {
            forklift.SetPercent(-percent);
        }
    }
    forklift.Stop();

    //***********Turn Right  60 degrees, perp to Ramp************
    turnRight(50.0, 20.0);

    //***********Move Forward 15 inches, to ramp***
    moveForward(10.0, 20.0);

    // Counter turn?
    turnLeft(5.0, 20.0);

    // Alt: up ramp *********** move up ramp on right(?) side *******
    moveForward(22.0, 30.0);
    
    
    // move up ramp (continue to move until line) trying to detect line
    while (!lineEqual(lf_middle.Value(), MIDDLE_LF_WH, TOLERA_LF_Wh))
    {
        SD.FPrintf(fout,"leftv: %f, midv: %f, rigv: %f\n",lf_left.Value(),lf_middle.Value(),lf_right.Value());
        turnLeft(2.0, 15.0);
    }
    // stop for 5 seconds to say line was detected enter line following
    leftMotor.Stop();
    rightMotor.Stop();

    // after waiting from red needs to turn right towards the ramp and move up it
    // consider using arrows on ramp somehow to center robot?
    // control line follower either through time or T end

    int state = MIDDLE;
    bool left = false, right = false, middle = false;
    do
    {

        switch (state)
        {
        case LEFT:
        {
            // move left
            leftMotor.SetPercent(5.0);
            rightMotor.SetPercent(15.0);
            if (right)
            {
                state = RIGHT;
            }
            else if (middle)
            {
                state = MIDDLE;
            }
            break;
        }
        case RIGHT:
        {
            // move right
            leftMotor.SetPercent(15.0);
            rightMotor.SetPercent(5.0);
            if (left)
            {
                state = LEFT;
            }
            else if (middle)
            {
                state = MIDDLE;
            }
            break;
        }
        case MIDDLE:
        {
            // move middle
            leftMotor.SetPercent(15.0);
            rightMotor.SetPercent(15.0);
            if (left)
            {
                state = LEFT;
            }
            else if (right)
            {
                state = RIGHT;
            }
            break;
        }
        }
        // Sleep for 0.2 seconds to stop instant switching between 2 cases when 2 linefollowers detecting
        Sleep(0.2);
        int l = left, m = middle, r = right;
        SD.FPrintf(fout,"left: %d, middle: %d, right: %d\n",l,m,r);
        SD.FPrintf(fout,"leftv: %f, midv: %f, rigv: %f\n",lf_left.Value(),lf_middle.Value(),lf_right.Value());
        left = lineEqual(lf_left.Value(), LEFT_LF_WH, TOLERA_LF_Wh);
        right = lineEqual(lf_right.Value(), RIGHT_LF_WH, TOLERA_LF_Wh);
        middle = lineEqual(lf_middle.Value(), MIDDLE_LF_WH, TOLERA_LF_Wh);
        // if all three are true, then exit line following. !(left && right && middle)
        // exits if no line follower is detecting a line left || right || middle
    } while (left || right || middle);
    SD.FClose(fout);
    leftMotor.Stop();
    rightMotor.Stop();
    Sleep(1.5);
    // near vulcrum of stamp arm
    //move forward to setup for turn
    moveForward(2.0,15.0);
    //turn to stamp arm
    turnRight(80.0,15.0);
    //forklift semi-up
    forklift.SetPercent(-percent);
    Sleep(2.0);
    forklift.Stop();
    //move forward 0.5 inches
    moveForward(1.0, 15.0);
    //turn a little left to stay under at top
    turnLeft(5.0,15.0);
    //forklift to top
    forklift.SetPercent(-percent);
    while(forktop.Value() != BP){
    }
    forklift.Stop();
    //turn left
    turnLeft(30.0,15.0);
    //Move to push
    moveForward(0.5,15.0);
    //move back to get out of way
    moveBackward(0.5,15.0);
    // move to position to get forklift down
    turnRight(40.0,15.0);
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
    leftMotor.SetPercent(percent);
    rightMotor.SetPercent(percent);
    while ((leftEncoder.Counts() + rightEncoder.Counts()) / 2 < ticks)
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
    while ((rightEncoder.Counts() < ticks) && (leftEncoder.Counts() < ticks)){
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