#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <FEHIO.h>
#include <FEHMotor.h>

/**
 * hashtag defines for all known constants and enumerations
*/

/*********** Team Key **************/
#define Team_Key "D4JZ1MbPn"

/************** PID *****************/
float P = 0.7;
float I = 0.1;
float D = 0.25;
//turn into #define s later after successful testing.

/************** Ports **************/
//FEHMotors - Motor
#define Motor_Left_Port FEHMotor::Motor0
#define Motor_Right_Port FEHMotor::Motor2
#define Motor_Forklift_Port FEHMotor::Motor3
//AnalogInputPin - Line Follower | CdS
#define LF_Left_Port FEHIO::P3_2
#define LF_Middle_Port FEHIO::P1_2
#define LF_Right_Port FEHIO::P1_0
#define CdS_Port FEHIO::P0_0
//AnalogEncoder - Encoder
#define Encoder_Left_Port FEHIO::P3_0
#define Encoder_Right_Port FEHIO::P0_3
//DigitalInputPin - Button
#define Button_ForkTop_Port FEHIO::P0_7
#define Button_ForkBot_Port FEHIO::P3_4
#define Button_ForkFront_Port FEHIO::P3_5
//reserved null port
#define Null_Port FEHIO::P3_7
#define Null_Motor_Port FEHMotor::Motor1


/**** Line Follower Line Values ****/
//LF_Left
#define LF_Left_White_Upper 0.3
#define LF_Left_White_Lower 0.08
#define LF_Left_Yellow_Upper 0.26
#define LF_Left_Yellow_Lower 0.08
//LF_Right 
#define LF_Right_White_Upper 2.2
#define LF_Right_White_Lower 1.8
#define LF_Right_Yellow_Upper 1.8
#define LF_Right_Yellow_Lower 1.5
//LF_Middle
#define LF_Middle_White_Upper 1.5
#define LF_Middle_White_Lower 1.1
#define LF_Middle_Yellow_Upper 1.05
#define LF_Middle_Yellow_Lower 0.7

/****** CdS Sensor Color Values ******/
//Red
#define CdS_Red_Upper 0.5
#define CdS_Red_Lower 0.1
//Blue
#define CdS_Blue_Upper 1.1
#define CdS_Blue_Lower 0.7

/******* Motor Voltages **********/
//DriveTrain Motors
#define DT_Voltage 9.0
//Forklift Motor
#define Forklift_Voltage 5.0

/******* Button Values *******/
//Button Pressed
#define BP 0
//Button Not Pressed
#define BNP 1

/**** Side Enumeration *****/
enum Side{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

/********* ForkLift ***********/
//signage for up
#define ForkLift_Sign_Up 1

/****** Encoders / Motors ********/
//encoder thresholds
#define Encoder_Low_Threshold 0.15
#define Encoder_High_Threshold 2.25

// 12.73239545 ticks/in
#define TICKS_PER_INCH 12.732
#define INCHES_PER_TICK 0.0785398163234

// 0.8888889222 ticks/degree
#define TICKS_PER_DEGREE 0.88888
#define DEGREES_PER_TICK 1.0/TICKS_PER_DEGREE

#endif