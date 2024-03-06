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
#define ONE_TICK_DIF_DEGREE 0.615 /* - /\ + */
#define CDS_SENSOR_PORT FEHIO::P0_2


/**
 * @brief reports the current angle of the robot with a error of + or -1 degree
 * @return double degrees between -179.9 and 179.9
 */
double reportAngle();

/**
 * @brief uses both motors to turn the argued amount of degrees, less percise but turn on the spot. - turn left, + turn right. Also adjusts setDir for encoders (reset by end).
 * @param degrees number of degrees to turn, error can range from + or - 2 degrees. Don't send more than 180 in either direction
 * @return the returned value of reportAngle at the end of this.
 */
double turnBoth(double degrees);

/**
 * @brief uses one motor to turn the argued amount of degrees, more percise but wider turn. - turn left, + turn right
 * @param degrees number of degrees to turn, error can range from + or - 1 degrees. Don't send more than 180 in either direction
 * @return the returned value of reportAngle at the end of this.
 */
double turnOne(double degrees);

/**
 * @brief linear movement for the argued number of inches, - backwards + forwards, with in 0.1 inches. IMPT encoders are tick reset. Also adjusts setDir for encoders (reset by end) 
 * 
 * linear movement for the argued number of inches.
 * forward is positive values, backwards is negative values.
 * Resets the tick counts of the encoders, and does setDir or setDirection for the encoders, rebasing to true, true by the end.
 * 
 * @param inches the number of inches to move
 * @return the actual number of inches moved
 */
double move(double inches);


/*
Global Motors, LightSensor, and Encoder objects
Default constructors used to build, member objects are pointed to in main.
*/
Motors moto;
LightSensor lumy;
Encoders cody;

/*
Global output file for run data
*/
FEHFile * output;

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

    //  output file open
    output = SD.FOpen("MainEnc.txt","a");
    SD.FPrintf(output,"\n*******************************\n\tNew Run Section\n");

    //Code emulating encoder exploration task
    LCD.Clear();
    LCD.WriteLine("Moving 14 inches");
    Sleep(0.3);
    move(14.0);
    Sleep(0.3);
    LCD.Clear();
    LCD.WriteLine("Turning -90 degrees or left, on spot");
    Sleep(0.3);
    turnBoth(-90.0);
    Sleep(0.3);
    LCD.Clear();
    LCD.WriteLine("Moving 5 inches");
    Sleep(0.3);
    move(5.0);
    Sleep(0.3);
    LCD.Clear();
    LCD.WriteLine("Turning 90 degrees or right, on spot");
    Sleep(0.3);
    turnBoth(90.0);
    Sleep(0.3);
    LCD.Clear();
    LCD.WriteLine("Moving 4 inches");
    Sleep(0.3);
    move(4.0);
    Sleep(0.2);

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
    //*****END OF PROGRAM********
    SD.FPrintf(output,"\n\tEND OF SECTION\n**********************************\n");
    SD.FCloseAll();
    return 1;
}

double reportAngle(){
    double res = cody.getRightDifferLeft() * ONE_TICK_DIF_DEGREE;
    SD.FPrintf(output,"reportAngle Full: %0.4d \t",res);
    //linear time, find better way to do it later
    while(res <= -180.0 || res >= 180.0){
        if (res <= -180.0){
            res += 360;
        } else {
            res -= 360;
        }
    }
    SD.FPrintf(output,"reportAngle trunc: %0.4d\n",res);
    return res;
}

double turnBoth(double degrees){
    SD.FPrintf(output,"TurnBoth\n");
    double curDeg = reportAngle();
    double getToDeg = curDeg + degrees;
    if (getToDeg <= -180.0){
        getToDeg += 360.0;
    } else if (getToDeg >= 180.0){
        getToDeg -= 360.0;
    }
    SD.FPrintf(output,"curDeg: %0.4d, change: %0.4d, getTo: %0.4d\n",curDeg,degrees,getToDeg);
    if (degrees < 0){
        cody.setDir(false, true);
        moto.setPerc(-15.0,15.0);
    } else {
        cody.setDir(true,false);
        moto.setPerc(15.0,-15.0);
    }
    const float tolerance = 1.4;
    SD.FPrintf(output,"tolerance: %f\n",tolerance);
    long l,r;
    do {
        curDeg = reportAngle();
        cody.archives(l,r);
        SD.FPrintf(output,"curDeg: %0.4d, leftArc: %i, righArc: %i\n",curDeg,l,r);
    } while (getToDeg - tolerance <= curDeg && getToDeg <= getToDeg + tolerance);
    moto.stop();
    cody.setDir(true,true);
    //end of turnBoth
    return reportAngle();
}

double turnOne(double degrees){
    SD.FPrintf(output,"TurnOne\n");
    double curDeg = reportAngle();
    double getToDeg = curDeg + degrees;
    if (getToDeg <= -180.0){
        getToDeg += 360.0;
    } else if (getToDeg >= 180.0){
        getToDeg -= 360.0;
    }
    SD.FPrintf(output,"curDeg: %0.4d, change: %0.4d, getTo: %0.4d\n",curDeg,degrees,getToDeg);
    if (degrees < 0){
        moto.setPerc(0,15.0);
    } else {
        moto.setPerc(15.0,0);
    }
    const float tolerance = 0.7;
    SD.FPrintf(output,"tolerance: %f\n",tolerance);
    long l,r;
    do {
        curDeg = reportAngle();
        cody.archives(l,r);
        SD.FPrintf(output,"curDeg: %0.4d, leftArc: %i, righArc: %i\n",curDeg,l,r);
    } while (getToDeg - tolerance <= curDeg && getToDeg <= getToDeg + tolerance);
    moto.stop();
    //end of turnOne
    return reportAngle();
}

double move(double inches){
    SD.FPrintf(output,"move, %0.4d inches\n",inches);
    double actualInches = 0;
    cody.resetTicks();
    int lefTic, rigTic;
    const int tolerance = 0.1;
    SD.FPrintf(output,"tolerance %0.2f\n",tolerance);
    if (inches < 0){
        cody.setDir(false, false);
        moto.setPerc(-15.0);
    } else {
        cody.setDir(true, true);
        moto.setPerc(15.0);
    }
    do {
        cody.ticks(lefTic, rigTic);
        actualInches = ((lefTic+rigTic)/2 ) * TICKS_INCHES;
        SD.FPrintf(output,"lefTic: %i, rigTic: %i, actuInch: %0.4d\n",lefTic,rigTic,actualInches);
    } while (inches - tolerance <= actualInches && actualInches <= inches + tolerance );
    moto.stop();
    cody.setDir(true,true);
    return actualInches;
}