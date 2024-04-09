#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>
#include <FEHRCS.h>

/*
hashtag defines
*/
// color values
// name_linefollower_color value
#define LEFT_LF_YLW 0.15
#define RIGHT_LF_YLW 1.65
#define MIDDLE_LF_YLW 0.86
#define TOLERA_LF_YLW 0.1
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
DigitalInputPin forkTop(BUTTON_FORKTOP_PORT);
DigitalInputPin forkBottom(BUTTON_FORKBOT_PORT);

int main()
{

    LCD.Clear();
    LCD.WriteLine("Starting Test");
    rightEncoder.SetThresholds(0.15, 2.35);
    rightEncoder.ResetCounts();
    leftEncoder.SetThresholds(0.15, 2.35);
    leftEncoder.ResetCounts();

    moveForward(5.0,30.0);

    


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
    rightMotor.SetPercent(percent + 0.9);
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