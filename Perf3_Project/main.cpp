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
    


    //****Main algorithm*****




}