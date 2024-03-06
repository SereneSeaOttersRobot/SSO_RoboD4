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

#define LEFT_MOTOR_PORT FEHMotor::Motor0
#define RIGHT_MOTOR_PORT FEHMotor::Motor2
#define VOLTAGE 9.0
#define LEFT_ENCODER_PORT FEHIO::P0_0
#define RIGHT_ENCODER_PORT FEHIO::P0_1
#define LOW_THRESHOLD 0.15
#define HIGH_THRESHOLD 2.25
//one tick is 0.07854
#define TICKS_INCHES 0.07854
#define CDS_SENSOR_PORT FEHIO::P0_2


/*
Global Motors, LightSensor, and Encoder objects
Default constructors used to build, member objects are pointed to in main.
*/
Motors moto;
LightSensor lumy;
Encoders cody;


int main(){
    //Finalize creation of global objects
    // Moto
    FEHMotor tempmotorleft(LEFT_MOTOR_PORT, VOLTAGE);
    FEHMotor tempmotorright(RIGHT_MOTOR_PORT,VOLTAGE);
    moto.left = &tempmotorleft;
    moto.right = &tempmotorright;
    moto.setMotorDirection(true, false);
    // Lumy
    AnalogInputPin tempcdssensor(CDS_SENSOR_PORT);
    lumy.cds_sensor = &tempcdssensor;
    //  currently using defualt light values for red and blue, and default tolerance, check these in the header / class file of LightSensor

    //  Cody
    AnalogEncoder tempencoderleft(LEFT_ENCODER_PORT);
    AnalogEncoder tempencoderright(RIGHT_ENCODER_PORT);
    cody.left = &tempencoderleft;
    cody.right = &tempencoderright;
    cody.setThresholds(LOW_THRESHOLD,HIGH_THRESHOLD);

    /*
    Wait on start light
    move to shallow ramp
    move up shallow ramp
    move to ticket light
    read and determine light detected at ticket light
    display determined color to screen
    move to correct colored ticket kiosk button
    press ticket kiosk button
    then move back to shallow ramp
    move down shallow ramp
    */

   /**
    * *Wait on start light
    * wait on DetectRed
    * *Center on ramp
    * Turn -90 degrees
    * move 4 inches 
    * Turn 45 degrees
    * *Move to and up ramp
    * move 10 inches
    * *Turn and move to light
    * Turn -45 degrees
    * Move 10 inches
    * *Detect, determine, and display light color
    * LColor color = LEMPTY
    * while (color == LEMPTY){
    * color = lumy.detectColor();
    * }
    * if (color == LRED){
    * display red
    * } else if (color == LBLUE){
    * display blue
    * } else {
    * display error
    * }
    * *Move to correct ticket kiosk button
    * turn -135 degrees
    * if color == LRED
    *   move 6 inches
    * else
    *   move 3 inches
    * turn 90 degrees
    * move 3 inches
    * *Press ticket kiosk button
    * move 1 inch
    * *Move straight back to be perpendicular with wall neighbor shallow ramp
    * move -8 inches
    * turn -90 degrees
    * *move to hit wall
    * move forward until front is pressed
    * *align to shallow ramp
    * move -1 inch
    * turn -90 degrees
    * *move down ramp
    * move 10 inches
    * 
    */
}

