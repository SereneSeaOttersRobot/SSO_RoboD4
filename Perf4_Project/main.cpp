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
 * @brief Main function for performance checkpoint 4.
 * 
 * Waits until it detects red at the start light.
 * Moves to and up the shallow ramp.
 * Finds the white line.
 * Line follows the line over to the Stamp Task.
 * Moves underneath and lifts/pushes the Stamp arm to Stamp the passport.
 * Moves to let the Stamp arm back down.
 * And ends...
 */
int main()
{   

    

    while (1){
        if (forkbottom.Value()==0){
            LCD.WriteLine("Button pressed");
        }
    }
    forklift.Stop();
    //open file to output collected testing data
    FEHFile *fout = SD.FOpen("PERFTR.txt","w");

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

    //Test code
    //move forward 10inches and stop
    if (0){
        moveForward(10.0,15.0);
        return 1;
    }

    //test smart move function going forward 6 inches and then stopping
    if (0){
        move(6.0, 20.0);
        return 1;
    }
    
    if(1){
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
    float percent = -90.0; //forklift percent. +values go up, -values go down.
    //percent = -90.0 means that forklift will move down here.
    forklift.SetPercent(percent);
    //if forklift button at top is currently pressed, Sleep for 0.2 seconds for forklift to unpress it
    if (forktop.Value() == BP){
        Sleep(0.2);
    }
    //while forkbottom is not pressed, move forklift until it is pressed.
    while (forkbottom.Value() == BNP)
    {
        //if top button is hit (even after moving off it initially), then switch the forklift movement direction
        if (forktop.Value() == BP)
        {
            forklift.SetPercent(-percent);
        }
    }
    //forklift bottom button is pressed, so stop forklift motor
    forklift.Stop();


    /////////////// Getting to and up shallow ramp ////////////////

    //Turn robot by 50 degrees in direction of the shallow ramp
    turnRight(50.0, 20.0);


    //Move robot forward
    moveForward(10.0, 30.0);

    // Left to correct up ramo
    turnLeft(7.5,30.0);

    // Drive up ramp

    moveForward(22.0,30.0);


    
    ////////////////// Finding Line //////////////////////

    //find white line with middle line follower by spinning onto it
    while (!lineEqual(lf_middle.Value(), MIDDLE_LF_WH, TOLERA_LF_Wh)) //while middle has not detected line
    {   
        //Print line followers' values to output file.
        SD.FPrintf(fout,"leftv: %f, midv: %f, rigv: %f\n",lf_left.Value(),lf_middle.Value(),lf_right.Value());
        //move two degrees to the left
        turnLeft(2.0, 15.0);
        //in while condition, check if middle detected line yet
    }
    
    //Deprecated code \/
    leftMotor.Stop();
    rightMotor.Stop();

    //////////////// Line follower State Machine //////////////////

    //state is the current line position state, determined by line follower values and updated during non-self states.
    int state = MIDDLE;
    //initially in middle state as coming from 'Finding Line' where middle was used to find the line.
    //boolean variables to hold line follower detection statuses.
    bool left = false, right = false, middle = false;
    //do...while any line follower boolean variables are not false. i.e. while any line follower is detecting a line.
    do
    {
        //switch statement that executes LEFT, MIDDLE, or RIGHT.
        switch (state)
        {
        case LEFT: //move left, update state to right or middle if applicable.
        {
            //move forward while leaning to the left
            leftMotor.SetPercent(5.0);
            rightMotor.SetPercent(15.0);
            //update state based on current detection of right and/or middle line followers. Bias for right
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
        case RIGHT: //move right, update state to left or middle if applicable
        {
            //move forward while leaning to the right
            leftMotor.SetPercent(15.0);
            rightMotor.SetPercent(5.0);
            //update state based on current detection of left and/or middle line followers. Bias for left
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
        case MIDDLE: //move forward, update state to left or right if applicable
        {
            //move straight forward
            leftMotor.SetPercent(15.0);
            rightMotor.SetPercent(15.0);
            //update state based on current detection of left and/or right line followers. Unintentional bias for left
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
        //int variables to represent boolean variables as 0s or 1s for file output
        int l = left, m = middle, r = right;
        SD.FPrintf(fout,"left: %d, middle: %d, right: %d\n",l,m,r); //outputting 'boolean' variables to file
        SD.FPrintf(fout,"leftv: %f, midv: %f, rigv: %f\n",lf_left.Value(),lf_middle.Value(),lf_right.Value()); //outputting line follower values to file
        //Updating boolean variables with current linefollowers' detection statuses
        left = lineEqual(lf_left.Value(), LEFT_LF_WH, TOLERA_LF_Wh);
        right = lineEqual(lf_right.Value(), RIGHT_LF_WH, TOLERA_LF_Wh);
        middle = lineEqual(lf_middle.Value(), MIDDLE_LF_WH, TOLERA_LF_Wh);

        // if all three are true, then exit line following. !(left && right && middle)
        // exits if no line follower is detecting a line left || right || middle
    } while (left || right || middle);
    //close output file stream
    SD.FClose(fout);
    //stop motors as linefollowing has ended
    leftMotor.Stop();
    rightMotor.Stop();
    //visual cue for linefollowing has ended by sleeping for 1.5 seconds. ? Use buzzer class for sounds instead ?
    Sleep(1.5);
    
    /////////////////// Moving to and under Stamp Arm ///////////////////

    //move forward to setup for turn
    moveForward(2.0,15.0);
    //turn to stamp arm
    turnRight(80.0,15.0);

    ////////////////// Lifting and Pushing Stamp Arm //////////////////////

    //forklift up for a few seconds
    forklift.SetPercent(-percent); //currently percent = -90.0
    Sleep(2.0);
    forklift.Stop();

    //move forward 1.15 inches, to get further under stamp arm
    moveForward(1.15, 15.0);

    //turn a little left so to stay under stamp arm when near top of forklift height.
    turnLeft(5.0,15.0);

    //forklift go up for two seconds
    forklift.SetPercent(-percent);
    Sleep(2.0);
    forklift.Stop();
    
    // Turn slightly and go forward more to get more under
    turnLeft(5.0,15.0);
    moveForward(0.5,15.0);
    
    // Raise forklift all the way up
    forklift.SetPercent(-percent);
    while(forktop.Value() != BP){
    }
    forklift.Stop();

    //turn left to push stamp arm
    turnLeft(25.0,30.0);
    //Move forward to push stamp arm more
    moveForward(.75,50.0);

    ////////////////// Getting Stamp Arm Back Down /////////////////


    
    //move back to get out of the way of the stamp arm
    moveBackward(2,30.0);

    // turn to get to left of stamp
    turnLeft(30.0,30);

    //Drive forwards to get back in plane with arm
    moveForward(3.5,30.0);

    //turn right to hit stamp down
    turnRight(50.0,30);

    // Move back to let it fall
    moveBackward(5.0,30.0);

    
    //END
    LCD.Clear();
    LCD.WriteLine("Finished");
    return 1;
    }
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