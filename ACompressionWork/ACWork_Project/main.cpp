#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>
#include <Constants.h>
#include <CdSensor.h>
#include <LineFollower.h>
#include <ForkLift.h>
#include <DriveMotor.h>


/*
Function prototypes
*/


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

LineFollower lf_left = LineFollower(LF_Left_Port, LEFT);
LineFollower lf_middle = LineFollower(LF_Middle_Port, MIDDLE);
LineFollower lf_right = LineFollower(LF_Right_Port, RIGHT);

CdSensor CdS = CdSensor(CdS_Port);

DriveMotor leftMotor = DriveMotor(Motor_Left_Port, DT_Voltage, Encoder_Left_Port);
DriveMotor rightMotor = DriveMotor(Motor_Right_Port, DT_Voltage, Encoder_Right_Port);

ForkLift forklift = ForkLift(Motor_Forklift_Port, Forklift_Voltage);



/**
 * @brief Main function for A compression work testing with proteus.
 * 
 */
int main()
{   
    forklift.setButtonPins(Button_ForkTop_Port, Button_ForkBot_Port, Button_ForkFront_Port);

    LCD.Clear();
    LCD.WriteLine("Starting Test");
    Sleep(2.0);

    forklift.toBottom();

    LCD.WriteLine("Test is over");
    return 1;
}




void moveForward(double inches, float percent)
{
    leftMotor.ResetCounts();
    rightMotor.ResetCounts();
    int ticks = (int)(inches * TICKS_PER_INCH / 1.22);
    leftMotor.SetPercent(percent + 0.75);
    rightMotor.SetPercent(percent);
    while (((leftMotor.Counts() + rightMotor.Counts()))/2  < ticks)
        ;
    leftMotor.Stop();
    rightMotor.Stop();
    

}

void moveBackward(double inches, float percent)
{
    leftMotor.ResetCounts();
    rightMotor.ResetCounts();
    int ticks = (int)(inches * TICKS_PER_INCH / 1.22);
    leftMotor.SetPercent(-percent - 1.4);
    rightMotor.SetPercent(-percent + 0.5);
    while ((leftMotor.Counts() + rightMotor.Counts()) / 2 < ticks)
        ;
    leftMotor.Stop();
    rightMotor.Stop();
}

void turnLeft(double degree, float percent)
{
    leftMotor.ResetCounts();
    rightMotor.ResetCounts();
    int ticks = (int)(degree * TICKS_PER_DEGREE);
    leftMotor.SetPercent(-percent);
    rightMotor.SetPercent(percent);
    while ((rightMotor.Counts() + leftMotor.Counts() / 2) < ticks)
        ;
    rightMotor.Stop();
    leftMotor.Stop();
}

void turnRight(double degree, float percent)
{
    leftMotor.ResetCounts();
    rightMotor.ResetCounts();
    int ticks = (int)(degree * TICKS_PER_DEGREE);
    leftMotor.SetPercent(percent);
    rightMotor.SetPercent(-percent);
    while ((leftMotor.Counts() + rightMotor.Counts() / 2) < ticks)
        ;
    rightMotor.Stop();
    leftMotor.Stop();
}

void move(double inches, float percent){
    leftMotor.ResetCounts();
    rightMotor.ResetCounts();
    int dir = 1;
    if (percent < 0){
        dir = -1;
    }
    int ticks = (int)(inches * TICKS_PER_INCH / 1.22);
    int modifier = 1.0;
    leftMotor.SetPercent(percent);
    rightMotor.SetPercent(percent);
    int l, r;
    while ((rightMotor.Counts() < (ticks/2)) && (leftMotor.Counts() < (ticks)/2)){
        l = leftMotor.Counts();
        r = rightMotor.Counts();
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