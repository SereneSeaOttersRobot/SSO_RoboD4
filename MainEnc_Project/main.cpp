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
TODO LIST
Bumpers need declared
If servo is important for this round then servo needs to be setup
Adjust algorithm for servo if needed
Find out issue with lightsensor and fix it
Write code translation of algorithm
Determine exact measurements
Possibly find work around for imprecise turning, like zigzag
Encoder class needs changed to be compatible for both single and double signal encoders
*/



#define LEFT_MOTOR_PORT FEHMotor::Motor0
#define RIGHT_MOTOR_PORT FEHMotor::Motor2
#define VOLTAGE 9.0

// We changed the encoder ports there are not accurate 
#define LEFT_ENCODER_PORT FEHIO::P3_3
#define RIGHT_ENCODER_PORT FEHIO::P0_0
#define LOW_THRESHOLD 0.15
#define HIGH_THRESHOLD 2.25
//one tick is 0.07854
#define TICKS_INCHES 0.07854
//#define TICKS_INCHES 0.21
#define ONE_TICK_DIF_DEGREE 0.615 /* - /\ + */
#define CDS_SENSOR_PORT FEHIO::P0_2
//determine light color values
#define RED_LIGHT 0.273
#define BLUE_LIGHT 0.9
#define TOLERANCE_LIGHT 0.075
//use of double encoders
#define DOUBLE_SIGNAL false
#define LEFT_D_ENCODER_PORT FEHIO::P0_3
#define RIGHT_D_ENCODER_PORT FEHIO::P0_4


#define TICKS_PER_INCH 12.732
//12.73239545 ticks/in

#define TICKS_PER_DEGREE 0.88888
// 0.8888889222 ticks/degree


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
    moto.setMotorDirection(true, true);
    // Lumy
    AnalogInputPin tempcdssensor(CDS_SENSOR_PORT);
    lumy.cds_sensor = &tempcdssensor;
    //  currently using defualt light values for red and blue, and default tolerance, check these in the header / class file of LightSensor
    //change to defined ones in this file
    lumy.setLights(RED_LIGHT,BLUE_LIGHT);
    lumy.setTolerance(TOLERANCE_LIGHT);
    //  Cody
    AnalogEncoder tempencoderleft(LEFT_ENCODER_PORT);
    AnalogEncoder tempencoderright(RIGHT_ENCODER_PORT);
    AnalogEncoder tempencoderleftD(LEFT_D_ENCODER_PORT);
    AnalogEncoder tempencoderrightD(RIGHT_D_ENCODER_PORT);
    cody.left = &tempencoderleft;
    cody.right = &tempencoderright;
    
    cody.setThresholds(LOW_THRESHOLD,HIGH_THRESHOLD);

    //  output file open
    output = SD.FOpen("MainEnc.txt","w");
    SD.FPrintf(output,"\n*******************************\n\tNew Run Section\n");

    //software problems, file storage may be overloaded with number of report degrees being called by turns
    //tolerances for turns may be too loose
    //...


    // Lukes stuff for checkpoint 2

    if (true){
        //set up encoders and CDS Cell
        AnalogEncoder rightEncoder(FEHIO::P0_3);
        AnalogEncoder leftEncoder(FEHIO::P3_0);
        LCD.Clear();
        LCD.WriteLine("Starting Test");
        rightEncoder.SetThresholds(0.15,2.35);
        rightEncoder.ResetCounts();
        leftEncoder.SetThresholds(0.15,2.35);
        leftEncoder.ResetCounts();

        AnalogInputPin sensor_cds(FEHIO::P0_0);
        float x=0.5;
        float y=0.1;
        int light = 0;
        int lightColor = 0;

        DigitalInputPin microF(FEHIO::P2_0);
        int fBump = 0;
        

        
        ///////////////// Wait for red //////////////////////////////
        float lightValue = sensor_cds.Value();

        while (lightValue > x || lightValue < y )
        {
        lightValue=sensor_cds.Value();
        LCD.WriteLine(lightValue);
        Sleep (0.1);
        }
        LCD.Clear();
        


        //////////////////// Turn right (45 degrees) ////////////////////////////////
        while( (rightEncoder.Counts() + leftEncoder.Counts() / 2) < (TICKS_PER_DEGREE*50) )
        {
            moto.setPerc(15,-15);
        }
        // stop everything and reset encoder
        moto.setPerc(0,0);
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();


        /////////////////Drive Forwards for 24 inches //////////////////////////////
        while( ((rightEncoder.Counts() + leftEncoder.Counts())/2 ) < (TICKS_PER_INCH*24) )
        {
            moto.setPerc(30,30);
            LCD.WriteLine(rightEncoder.Counts());
            Sleep(1);
        }
        // stop everything and reset encoder
        moto.setPerc(0,0);
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();


        //////////////////// Turn left (45 degrees) ////////////////////////////////
        while( (rightEncoder.Counts() + leftEncoder.Counts() / 2) < (TICKS_PER_DEGREE*45) )
        {
            moto.setPerc(-15,15);
        }
        // stop everything and reset encoder
        moto.setPerc(0,0);
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();
        

        /////////////////Drive Forwards for 15 inches //////////////////////////////
        while( (rightEncoder.Counts() + leftEncoder.Counts() / 2) < (TICKS_PER_INCH*15) )
        {
            moto.setPerc(30,30);
        }
        // stop everything and reset encoder
        moto.setPerc(0,0);
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();


        moto.setPerc(15,15);
        fBump = microF.Value();
        /////////////////Drive Forwards until we find the light or hit wall //////////////////////////////
        while(light == 0 || fBump == 1)
        {
            
            // check if we detect a light
            lightValue=sensor_cds.Value();
            if(lightValue < 2)
            {
                light = 1;
            }
            fBump = microF.Value();
            Sleep(1);
        }
        // stop everything and reset encoder
        moto.setPerc(0,0);
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();



         /////////////////found light go detect color//////////////////////////////
        if(light == 1)
        {
            /////////////////Drive Forwards for 1/2 inches //////////////////////////////
            while( (rightEncoder.Counts() + leftEncoder.Counts() / 2) < (TICKS_PER_INCH*.5) )
            {
                moto.setPerc(15,15);
            }
            // stop everything and reset encoder
            moto.setPerc(0,0);
            leftEncoder.ResetCounts();
            rightEncoder.ResetCounts();

            lightValue = sensor_cds.Value();
            if( lightValue > .35 && lightValue < .55)
            {
                lightColor = 1;
                LCD.WriteLine("Found RED light");
            }
            else if(lightValue > .25 && lightValue < .35)
            {
                lightColor = 2;
                LCD.WriteLine("Found BLUE light");
            }

        }
        
         /////////////////did not find light guess and pray //////////////////////////////
        if(light == 0)
        {
            /////////////////Drive backwards for 10 inches //////////////////////////////
            while( ((rightEncoder.Counts() + leftEncoder.Counts())/2 ) < (TICKS_PER_INCH*10) )
            {
                moto.setPerc(-20,-20);
                LCD.WriteLine(rightEncoder.Counts());
                Sleep(1);
            }
            // stop everything and reset encoder
            moto.setPerc(0,0);
            leftEncoder.ResetCounts();
            rightEncoder.ResetCounts();



            //////////////////// Turn right (45 degrees) ////////////////////////////////
            while( (rightEncoder.Counts() + leftEncoder.Counts() / 2) < (TICKS_PER_DEGREE*50) )
            {
                moto.setPerc(15,-15);
            }
            // stop everything and reset encoder
            moto.setPerc(0,0);
            leftEncoder.ResetCounts();
            rightEncoder.ResetCounts();

            /////////////////Drive forwards until bump swtich hits //////////////////////////////
            while( microF.Value() == 1) 
            {
                moto.setPerc(-20,-20);
                LCD.WriteLine(rightEncoder.Counts());
                Sleep(1);
            }
            Sleep(500);
            // stop everything and reset encoder
            moto.setPerc(0,0);
            leftEncoder.ResetCounts();
            rightEncoder.ResetCounts();


        }





    }
    if (false){
        cody.resetTicks();
        int l,r;
        LCD.Clear();
        LCD.WriteLine("TurnRight for 50");
        moto.setPerc(15,-15);
        do {
            cody.ticks(l,r);
        } while ((l+r)/2 < 15);
        moto.stop();
        LCD.WriteLine("move for 25");
        cody.resetTicks();
        moto.setPerc(15,15);
        do {
            cody.ticks(l,r);
        } while ((l+r)/2 < 25);
        moto.stop();
        LCD.WriteLine("turnleft for 50");
        cody.resetTicks();
        moto.setPerc(-15,15);
        do {
            cody.ticks(l,r);
        } while ((l+r)/2 < 15);
        moto.stop();

        LCD.WriteLine("move for 75");
        cody.resetTicks();
        moto.setPerc(-15,15);
        do {
            cody.ticks(l,r);
        } while ((l+r)/2 < 75);
        moto.stop();
        
    }


    /*
    Test: just that move works for 1 inch
    */
    if (false){
        LCD.Clear();
        LCD.WriteLine("Moving forward 5 inch");
        Sleep(2.0);
        double afterInches = move(5.0);
        LCD.Write("Inches moved: ");
        LCD.WriteLine(afterInches);
        int l,r;
        cody.ticks(l,r);
        LCD.WriteLine((l+r)/2);
        Sleep(8.0);
    }

    /*
    Test: move works for -1 inch
    */
    if (false){
        LCD.Clear();
        LCD.WriteLine("Moving backwards one inch");
        Sleep(2.0);
        double afterInches = move(-1.0);
        LCD.Write("Inches moved: ");
        LCD.WriteLine(afterInches);
        Sleep(8.0);
    }

    /*
    Test: turnBoth for 30 degrees
    */
    if (false) {
        LCD.Clear();
        LCD.Write("Test turnBoth 30 degrees from cur: ");
        double curDeg = reportAngle();
        LCD.WriteLine(curDeg);
        Sleep(2.0);
        double newDeg = turnBoth(30.0);
        LCD.Write("New angle: ");
        LCD.WriteLine(newDeg);
        LCD.Write("Change: ");
        LCD.WriteLine((newDeg-curDeg));
        Sleep(9.0);
    }

    /*
    Test: Detect Red start light, then move forward by 0.1 until Detect Red is false, wait on that position for 15 seconds, then move back half the traveled and try to detect red
    */
    if (false){
        do {
            LCD.Clear();
            LCD.WriteLine("Waiting to detect red");
            LCD.WriteLine(lumy.lightValue());
            Sleep(0.2);
        } while (!lumy.detectRed());
        LCD.Clear();
        LCD.WriteLine("Detected red");
        LCD.WriteLine(lumy.lightValue());
        LCD.WriteLine("Inches moved 0.0");
        Sleep(0.5);
        double inchesMoved = 0.0;
        while (lumy.detectRed()){
            LCD.Clear();
            LCD.WriteLine("Move 0.21 inches");
            inchesMoved += move(0.21);
            LCD.Write("Total Moved: ");
            LCD.WriteLine(inchesMoved);
            LCD.Write("LigthValue: ");
            LCD.WriteLine(lumy.lightValue());
            Sleep(0.3);
        }
        LCD.Clear();
        LCD.Write("Final inches moved: ");
        LCD.WriteLine(inchesMoved);
        LCD.Write("Final lightVal: ");
        LCD.WriteLine(lumy.lightValue());
        Sleep(5.0);
        LCD.WriteLine("move back half of moved");
        inchesMoved += move(-(inchesMoved/2));
        LCD.WriteLine(inchesMoved);
        LCD.Write("Detected Red? ");
        if (lumy.detectRed()){
            LCD.WriteLine("yes");
        } else {
            LCD.WriteLine("no");
        }
        Sleep(5.0);
    }

    /*
    Test: determine if using the reportAngle value returned after calling turn, that we can turn back to original position exactly
    */
    if (false){
        LCD.Clear();
        LCD.WriteLine("Testing how exact we can turn to previous direction");
        double curAng = reportAngle();
        LCD.Write("Current Angle: ");
        LCD.WriteLine(curAng);
        LCD.WriteLine("Turn 90 degrees on spot");
        Sleep(4.0);
        double newAng = turnBoth(90.0);
        LCD.Clear();
        LCD.Write("New angle: ");
        LCD.WriteLine(newAng);
        //assuming curAng was 0.0, so turn -newAng
        LCD.WriteLine("Turn the negative of that");
        Sleep(4.0);
        newAng = turnBoth(-newAng);
        LCD.Clear();
        LCD.Write("New angle: ");
        LCD.WriteLine(newAng);
        LCD.WriteLine("End of this");
        Sleep(8.0);
    }

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
    } while (getToDeg - tolerance >= curDeg || curDeg >= getToDeg + tolerance);
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
    } while (getToDeg - tolerance >= curDeg || curDeg >= getToDeg + tolerance);
    moto.stop();
    //end of turnOne
    return reportAngle();
}

double move(double inches){
    SD.FPrintf(output,"move, %0.4d inches\n",inches);
    double actualInches = 0;
    cody.resetTicks();
    int lefTic, rigTic;
    const double tolerance = 0.11;
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
    } while (inches - tolerance >= actualInches || actualInches >= inches + tolerance );
    moto.stop();
    cody.setDir(true,true);
    return actualInches;
}

