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

#define TICKS_PER_INCH 12.732
//12.73239545 ticks/in

#define TICKS_PER_DEGREE 0.88888
// 0.8888889222 ticks/degree


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

FEHMotor leftMotor(MOTOR_LEFT_PORT,VOLTAGE_MOTORS);
FEHMotor rightMotor(MOTOR_RIGHT_PORT,VOLTAGE_MOTORS);
AnalogInputPin sensor_cds(LIGHTSENSOR_CDS_PORT);
AnalogEncoder leftEncoder(ENCODER_LEFTS_PORT);
AnalogEncoder rightEncoder(ENCODER_RIGHTS_PORT);
AnalogInputPin lf_left(LIGHTFOLLOWER_LEFT_PORT);
AnalogInputPin lf_middle(LIGHTFOLLOWER_MIDDLE_PORT);
AnalogInputPin lf_right(LIGHTFOLLOWER_RIGHT_PORT);
FEHMotor forklift(MOTOR_FORKLIFT_PORT,VOLTAGE_FORKLIFT);

int main(){

        LCD.Clear();
        LCD.WriteLine("Starting Test");
        rightEncoder.SetThresholds(0.15,2.35);
        rightEncoder.ResetCounts();
        leftEncoder.SetThresholds(0.15,2.35);
        leftEncoder.ResetCounts();


        float x=0.5;
        float y=0.1;
        int light = 0;
        int lightColor = 0;

        

        
        ///////////////// Wait for red //////////////////////////////
        float lightValue = sensor_cds.Value();

        while (lightValue > x || lightValue < y )
        {
        lightValue=sensor_cds.Value();
        LCD.WriteLine(lightValue);
        Sleep (0.1);
        }
        LCD.Clear();
        

         /////////////////Drive Forwards for 23 inches //////////////////////////////
        while( ((rightEncoder.Counts() + leftEncoder.Counts())/2 ) < (TICKS_PER_INCH*17.5) )
        {
            leftMotor.SetPercent(30);
            rightMotor.SetPercent(30);
            LCD.WriteLine(rightEncoder.Counts());
            Sleep(1);
        }
        // stop everything and reset encoder
        leftMotor.SetPercent(0);
        rightMotor.SetPercent(0);
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();


        //////////////////// Turn right (150 degrees) ////////////////////////////////
        while( (rightEncoder.Counts() + leftEncoder.Counts() / 2) < (TICKS_PER_DEGREE*150) )
        {
            leftMotor.SetPercent(-15);
            rightMotor.SetPercent(15);
        
        }
        // stop everything and reset encoder
        leftMotor.SetPercent(0);
        rightMotor.SetPercent(0);
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();

    /////////////////Drive Forwards for 10 inches //////////////////////////////
        while( ((rightEncoder.Counts() + leftEncoder.Counts())/2 ) < (TICKS_PER_INCH*4) )
        {
            leftMotor.SetPercent(30);
            rightMotor.SetPercent(30);
            LCD.WriteLine(rightEncoder.Counts());
            Sleep(1);
        }
        // stop everything and reset encoder
        leftMotor.SetPercent(0);
        rightMotor.SetPercent(0);
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();

       


      
}

bool lineEqual(float x, float y, float toler){
    bool res = false;
    if (y-toler <= x && x <= y+toler){
        res = true;
    }
    return res;
}