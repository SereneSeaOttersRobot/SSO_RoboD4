#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <FEHIO.h>
#include <FEHMotor.h>

/**
 * hashtag defines for all known constants and enumerations
*/

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


/**** Line Follower Line Values ****/
//LF_Left
#define LF_Left_White_Upper 0
#define LF_Left_White_Lower 0
#define LF_Left_Yellow_Upper 0
#define LF_Left_Yellow_Lower 0
//LF_Right 
#define LF_Right_White_Upper 0
#define LF_Right_White_Lower 0
#define LF_Right_Yellow_Upper 0
#define LF_Right_Yellow_Lower 0
//LF_Middle
#define LF_Middle_White_Upper 0
#define LF_Middle_White_Lower 0
#define LF_Middle_Yellow_Upper 0
#define LF_Middle_Yellow_Lower 0

/****** CdS Sensor Color Values ******/
//Red
#define CdS_Red_Upper 3.3
#define CdS_Red_Lower 0.0
//Blue
#define CdS_Blue_Upper 3.3
#define CdS_Blue_Lower 0.0

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

#endif