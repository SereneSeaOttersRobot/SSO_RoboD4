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
#include <FEHRCS.h>
#define SLOWSPEED 2.0
#define FASTSPEED 2.5
#define TURNSPEED 1.6


/*
Function prototypes
*/


/**
 * Drives by the given number of inches with velocity given by the speed.
 * @param inches the distance to traverse as inches
 * @param speed the velocity at which to travel, negative to go backwards.
*/
void Drive(float inches, float speed);

/**
 * Turns this by the given degrees at the given speed in the given direction.
 * Turns left if direction is set to left and right if direction is set to right.
 * Direction can't be set to middle
 * @param degrees the amount of degrees to turn this by.
 * @param speed the speed at which to turn this
 * @param direction the side to turn into.
*/
void Turn(float degrees, float speed, Side direction);

/**
 * Starts line following on the given color.
 * Requires lf_middle to be on line.
 * @param colorToFollow white or yellow colored line to follow
*/
void LineFollow(Color colorToFollow);

/**
 * Test function for supporting maneul determination of PID constant values
*/
void PIDTesting();

/*
Global objects
*/

LineFollower lf_left = LineFollower(LF_Left_Port, LEFT);
LineFollower lf_middle = LineFollower(LF_Middle_Port, MIDDLE);
LineFollower lf_right = LineFollower(LF_Right_Port, RIGHT);

CdSensor CdS = CdSensor(CdS_Port);

DriveMotor leftMotor = DriveMotor(Motor_Left_Port, DT_Voltage);
DriveMotor rightMotor = DriveMotor(Motor_Right_Port, DT_Voltage);
AnalogEncoder leftEncoder = AnalogEncoder(Encoder_Left_Port);
AnalogEncoder rightEncoder = AnalogEncoder(Encoder_Right_Port);

DriveTrain drivetrain = DriveTrain(leftMotor, rightMotor);

ForkLift forklift = ForkLift(Motor_Forklift_Port, Forklift_Voltage);



/**
 * @brief Main function for A compression work testing with proteus.
 * 
 */
int main()
{   
    forklift.setButtonPins(Button_ForkTop_Port, Button_ForkBot_Port, Button_ForkFront_Port);
    leftEncoder.SetThresholds(Encoder_Low_Threshold, Encoder_High_Threshold);
    rightEncoder.SetThresholds(Encoder_Low_Threshold, Encoder_High_Threshold);
    if (0){
        //turning and finding the line
        leftMotor.SetPercent(15.);
        rightMotor.SetPercent(-15.);
        while (!lf_middle.onWhite()){
            //LCD.WriteLine(lf_right.Value());
            //LCD.WriteLine(lf_middle.Value());
            //LCD.WriteLine(lf_left.Value());
            //Sleep(0.3);
            //LCD.Clear();
        }
        leftMotor.Stop();
        rightMotor.Stop();
        Color color = Color::White;
        LineFollow(color);  //stops one if doesnt find the lin
    }
    
    if (1){
    RCS.InitializeTouchMenu(Team_Key);
    const int Lever = RCS.GetCorrectLever(); //follows the side enumeration

    ///////////////////////////////Step 1, start with initiating at light////////////////////////
    while (CdS.notRed()){
        LCD.WriteLine(CdS.Value());
    }

    //////////////////////////Step 2, moving back to hit starting button/////////////////////////
    Drive(3.0,-SLOWSPEED);

    ////////////////////////Step 3 moving to and completing fuel lever///////////////////////////
    Drive(20.0,FASTSPEED);

        //moving to correct fuel lever
        if (Lever == LEFT){
            Drive(0.25,SLOWSPEED);
            Turn(138.,TURNSPEED,LEFT);
            Drive(1.25,SLOWSPEED);
        }
        else if (Lever == MIDDLE){
            Turn(45.,TURNSPEED,LEFT);
            Drive(4.0,SLOWSPEED);
            Turn(90.,TURNSPEED,LEFT);
            Drive(1.,SLOWSPEED);
        }
        else if (Lever == RIGHT){
            Turn(45.,15.,LEFT);
            Drive(5.75,SLOWSPEED);
            Turn(75.,15.,LEFT);
            Drive(1.5,SLOWSPEED);
        }

    //moving forklift and maneuvering to flip fuel switch up and down
    forklift.down();
    Sleep(2.0);
    forklift.Stop();
    Drive(2.,-SLOWSPEED);
    //wait 5 seconds
    Sleep(3.5);
    //move arm lower than fuel lever.
    forklift.down();
    Sleep(1.0);
    forklift.Stop();
    Drive(2.25, SLOWSPEED);
    forklift.up();
    Sleep(2.0);
    forklift.Stop();
    Drive(2.0,-SLOWSPEED);

    /////////////////////////Step 4 moving to ticket kiosk light/////////////////////////////
    Turn(90.,TURNSPEED,LEFT);
    
    rightMotor.SetPercent(30.);
    leftMotor.SetPercent(30.);
    while (forklift.front()==BNP){}
    rightMotor.Stop();
    leftMotor.Stop();

    Turn(100.,TURNSPEED,LEFT);
    Drive(34.,FASTSPEED);
    Turn(60.,TURNSPEED,LEFT);
    Drive(24.,FASTSPEED);
    
    //turning and finding the line
    leftMotor.SetPercent(15.);
    rightMotor.SetPercent(-15.);
    while (!lf_middle.onWhite()) //while middle has not detected line
    {   
        //Print line followers' values to output file.
        //SD.FPrintf(fout,"leftv: %f, midv: %f, rigv: %f\n",lf_left.Value(),lf_middle.Value(),lf_right.Value());
        //move two degrees to the left
        //Turn(2.0, 15.0,RIGHT);
        //in while condition, check if middle detected line yet
    }
    leftMotor.Stop();
    rightMotor.Stop();

    //following the line
    Color color = Color::White;
    LineFollow(color);  //stops one if doesnt find the line

    //maneuver to get to light
    Turn(10.0,TURNSPEED,LEFT);
    Drive(3.5,SLOWSPEED);
    
    ////////////////////////////STEP 5 reading light and moving to and pressing appropriate button/////////////////////////
    //determining blue and red and moving to appropriate button
    float lightValue;
    lightValue=CdS.Value();
        if (lightValue>0.6){
            //moving to blue button
            LCD.WriteLine("Blue light Found");
            forklift.up();
            Drive(1.,-SLOWSPEED);
            forklift.Stop();
            Drive(2.,-SLOWSPEED);
            Turn(140.,TURNSPEED,RIGHT);
            Drive(2.5,SLOWSPEED);
            Turn(100.,15.,LEFT);
            leftMotor.SetPercent(15.);
            rightMotor.SetPercent(15.);

            //waiting for contact
            while (forklift.front()==BNP){};

            leftMotor.Stop();
            rightMotor.Stop();
            Drive(0.2,SLOWSPEED);

            //starting to move towards luggage
            Drive(2.,-SLOWSPEED);
            Turn(90.,TURNSPEED,LEFT);
            Drive(3.,SLOWSPEED);
            Turn(90.,TURNSPEED,LEFT);

        }
        else if(lightValue<=0.6){
            //moving to red button
            LCD.WriteLine("Red Light Found");
            
            forklift.up();
            Drive(1.,-SLOWSPEED);
            forklift.Stop();
            Drive(2.,-SLOWSPEED);
            Turn(130.,SLOWSPEED,RIGHT);
            Drive(6.,SLOWSPEED);
            Turn(100.,SLOWSPEED,LEFT);

            leftMotor.SetPercent(15.);
            rightMotor.SetPercent(15.);
            
            //waiting for contact
            while (forklift.front()==BNP){};

            leftMotor.Stop();
            rightMotor.Stop();
            Drive(0.2,SLOWSPEED);

            //starting to move towards luggage
            Drive(2.,-SLOWSPEED);
            Turn(90.,TURNSPEED,LEFT);
            Drive(8.25,SLOWSPEED);
            Turn(90.,TURNSPEED,LEFT);
        }
    
    /////////////////////STEP 6 Moving to luggage drop off and dropping the luggage off/////////////////////////////
    //turning and moving towards the luggage drop off

    //move forward until front bumper is hit
    while (forklift.front() != BP){
        leftMotor.SetPercent(15.0);
        rightMotor.SetPercent(15.0);
    }
    leftMotor.Stop();
    rightMotor.Stop();

    //move back a little so bumper doesn't grind
    Drive(0.2, -SLOWSPEED);

    //lift forklift so to drop luggage
    forklift.toTop();

    //move forward to push in
    Drive(0.3,SLOWSPEED);

    //move back 1 inch
    Drive(1.3, -SLOWSPEED);

    ///////////////////STEP 7 Moving to passport and completing passport/////////////////////////////
    //turn towards line
    Turn(120.0,TURNSPEED,LEFT);
    Drive(13.,FASTSPEED);

    //turning left until line is found
    leftMotor.SetPercent(-15.);
    rightMotor.SetPercent(15.);
    while (!lf_middle.onWhite()) //while middle has not detected line
    {   
        //Print line followers' values to output file.
        //SD.FPrintf(fout,"leftv: %f, midv: %f, rigv: %f\n",lf_left.Value(),lf_middle.Value(),lf_right.Value());
        //move two degrees to the left
        //Turn(2.0, 15.0,RIGHT);
        //in while condition, check if middle detected line yet
    }
    leftMotor.Stop();
    rightMotor.Stop();
    //following the line
    LineFollow(color);  //stops one if doesnt find the line

    //maneuver to flip passport stamp
    //move forward to setup for turn
    Drive(2.0,SLOWSPEED);
    //turn to stamp arm
    Turn(80.0,TURNSPEED,RIGHT);

    ////////////////// Lifting and Pushing Stamp Arm //////////////////////

    //forklift up for a few seconds
    forklift.up(); //currently percent = -90.0
    Sleep(2.0);
    forklift.Stop();

    //move forward 1.15 inches, to get further under stamp arm
    Drive(1.15, SLOWSPEED);

    //turn a little left so to stay under stamp arm when near top of forklift height.
    Turn(5.0,TURNSPEED,LEFT);

    //forklift go up for two seconds
    forklift.up();
    Sleep(2.0);
    forklift.Stop();
    
    // Turn slightly and go forward more to get more under
    Turn(5.0,TURNSPEED,LEFT);
    Drive(0.5,SLOWSPEED);
    
    // Raise forklift all the way up
    forklift.toTop();

    //turn left to push stamp arm
    Turn(25.0,TURNSPEED,LEFT);
    //Move forward to push stamp arm more
    Drive(.75,FASTSPEED);

    ////////////////// Getting Stamp Arm Back Down /////////////////


    
    //move back to get out of the way of the stamp arm
    Drive(2,-FASTSPEED);

    // turn to get to left of stamp
    Turn(30.0,TURNSPEED,LEFT);

    //Drive forwards to get back in plane with arm
    Drive(3.5,FASTSPEED);

    //turn right to hit stamp down
    Turn(50.0,TURNSPEED,RIGHT);

    // Move back to let it fall
    Drive(5.0,-FASTSPEED);

    ////////////////////////////////STEP 8 finding line and going back to stop button//////////////////////////////
    Turn(140.,TURNSPEED,RIGHT);
    Drive(15.,FASTSPEED);
    //finding line and following it to ramp
    leftMotor.SetPercent(-15.);
    rightMotor.SetPercent(15.);
    while (!lf_middle.onWhite()) //while middle has not detected line
    {   
        //Print line followers' values to output file.
        //SD.FPrintf(fout,"leftv: %f, midv: %f, rigv: %f\n",lf_left.Value(),lf_middle.Value(),lf_right.Value());
        //move two degrees to the left
        //Turn(2.0, 15.0,RIGHT);
        //in while condition, check if middle detected line yet
    }
    leftMotor.Stop();
    rightMotor.Stop();
    //following the line
    LineFollow(color);  //stops one if doesnt find the line

    //////////// Move down ramp and back to button ///////////

    /////// move forklift to down position ////
    forklift.toBottom();
    //move forklift up a little to align with final button
    forklift.up();
    Sleep(1.0);
    forklift.Stop();

    //////// adjustment turn?
    //forward to setup for turn
    Drive(1.0,SLOWSPEED);
    //turn right to face down ramp
    Turn(40.0,TURNSPEED,RIGHT);

    //move down and off ramp
    Drive(15.0,SLOWSPEED);

    //turnRight for setup turn
    Turn(50.0,TURNSPEED,RIGHT);

    //move forward to align
    Drive(10.0, SLOWSPEED);

    //turn to align
    Turn(102.0,TURNSPEED,LEFT);

    leftMotor.SetPercent(15.0);
    rightMotor.SetPercent(15.0);
    //straight on shot, use fork button to find hit
    while (forklift.front() == BNP){ }
    leftMotor.Stop();
    rightMotor.Stop();

    //hit button harder
    Drive(0.5,SLOWSPEED);
    
    LCD.WriteLine("Test is over");
    return 1;
    }
}

void Drive(float inches, float speed){
    drivetrain.setSpeed(speed);
    drivetrain.Drive(inches, leftEncoder, rightEncoder);
}

void Turn(float degrees, float speed, Side direction){
    float actual = 0;
    if (direction == LEFT){
        actual = -speed;
    } else if (direction == RIGHT) {
        actual = speed; 
    }
    drivetrain.setSpeed(actual);
    drivetrain.Turn(degrees, leftEncoder, rightEncoder);
}

void LineFollow(Color colorToFollow){
    Color line = colorToFollow;
    bool left = false, middle = true, right = false;
    int state = MIDDLE;
    do {
        switch(state){
            case LEFT:{
                //set motors
                leftMotor.SetPercent(LF_Left_LeftMotor);
                rightMotor.SetPercent(LF_Left_RightMotor);
                //update state
                if (right){
                    state = RIGHT;
                } else if (middle){
                    state = MIDDLE;
                }
                break;
            }
            case MIDDLE:{
                //set motors
                leftMotor.SetPercent(LF_Middle_Motor);
                rightMotor.SetPercent(LF_Middle_Motor);
                //update state
                if (right){
                    state = RIGHT;
                } else if (left){
                    state = LEFT;
                }
                break;
            }
            case RIGHT:{
                //set motors
                leftMotor.SetPercent(LF_Right_LeftMotor);
                rightMotor.SetPercent(LF_Right_RightMotor);
                //update state
                if (left){
                    state = LEFT;
                } else if (middle){
                    state = MIDDLE;
                }
                break;
            }
            default:{
                //never gets run
            }
        }
        Sleep(0.05);
        left = lf_left.onColor(line);
        middle = lf_middle.onColor(line);
        right = lf_right.onColor(line);
    } while (left || middle || right);
    leftMotor.Stop();
    rightMotor.Stop();

}

void PIDTesting(){
    //use encoders to set PID values, use forklift buttons to select and set
    LCD.Clear();
    LCD.WriteLine("Use right encoder to add, left encoder to subtract");
    LCD.WriteLine("Use bottom to select previous, top to select next");
    LCD.WriteLine("Use front to accept ");
    Sleep(5.0);

    float items[6] = {PID.P, PID.I, PID.D, drivetrain.currentSpeed(), 0.0, 0.0};
    char labels[6][10] = {"P","I","D","Spe","Dis", "Start"};
    int view = 0;
    while (view < 5 && forklift.front() == BNP){
        leftEncoder.ResetCounts();
        rightEncoder.ResetCounts();
        bool nextPressed = false;
        float save = items[view];
        while (!nextPressed){
            if (forklift.front() == BP){
                items[view] = save;
                nextPressed = true;
            } else if (forklift.top() == BP){
                view++;
                nextPressed = true;
            } else if (forklift.bottom() == BP){
                view--;
                nextPressed = true;
            } else {
                int left = leftEncoder.Counts(), right = rightEncoder.Counts();
                if (view == 3){
                    save = items[view] + 0.1*(right - left);
                } else if (view == 4){
                    save = items[view] + 0.5*(right - left);
                } else {
                    save = items[view] + 0.01*(right - left);
                }
            }
            LCD.Clear();
            LCD.Write(labels[view]);
            LCD.Write(" : ");
            LCD.Write(items[view]);
            LCD.Write(" : ");
            LCD.WriteLine(save);
            Sleep(0.2);
        }
    }
    PID.P = items[0];
    PID.I = items[1];
    PID.D = items[2];
    Drive(items[4], items[3]);
    //no ending for you, recur call, end with power button
    PIDTesting();
}




/*void DriveTrainConstantTesting(){
    float tx = 0.0, ty = 0.0;
    LCD.Clear();
    using namespace FEHIcon;
    Icon kbsw[15]; //knobbles and switchs
    char label[15][20] = {"+","+","+","+","+","P","I","D","v","d","-","-","-","-","-"};
    DrawIconArray(kbsw,3,5,20,20,60,60,label,WHITE,WHITE);
    //get touchs for icons 0-4 ad 10-14
    //update labels for icons 5-9
    double p = 0.75, i=0.1, d=0.25, dis=1.0, spe=0.02;
    forklift.toBottom();
    forklift.up();
    Sleep(1.0);
    forklift.Stop();
    kbsw[5].ChangeLabelFloat((float)p);
    kbsw[6].ChangeLabelFloat((float)i);
    kbsw[7].ChangeLabelFloat((float)d);
    kbsw[8].ChangeLabelFloat((float)spe);
    kbsw[9].ChangeLabelFloat((float)dis);
    bool exitFlag = false;
    do {
        for (Icon k : kbsw){
            k.Draw();
        }
        if (forklift.top() == BP){
            exitFlag = true;
        } else {
            LCD.ClearBuffer();
            bool pressed = false;
            while (!LCD.Touch(&tx,&ty));
            while (LCD.Touch(&tx,&ty));
            
                if (kbsw[0].Pressed(tx,ty,1)){
                    p += 0.05;
                } else if (kbsw[1].Pressed(tx,ty,1)){
                    i += 0.05;
                } else if (kbsw[2].Pressed(tx,ty,1)){
                    d += 0.05;
                } else if (kbsw[3].Pressed(tx,ty,1)){
                    spe += 0.5;
                } else if (kbsw[4].Pressed(tx,ty,1)){
                    dis += 1.0;
                } else if (kbsw[10].Pressed(tx,ty,1)){
                    p -= 0.05;
                } else if (kbsw[11].Pressed(tx,ty,1)){
                    i -= 0.05;
                } else if (kbsw[12].Pressed(tx,ty,1)){
                    d -= 0.05;
                } else if (kbsw[13].Pressed(tx,ty,1)){
                    spe -= 0.5;
                } else if (kbsw[14].Pressed(tx,ty,1)){
                    dis -= 1.0;
                }
                kbsw[5].ChangeLabelFloat((float)p);
                kbsw[6].ChangeLabelFloat((float)i);
                kbsw[7].ChangeLabelFloat((float)d);
                kbsw[8].ChangeLabelFloat((float)spe);
                kbsw[9].ChangeLabelFloat((float)dis);
            
            
            Sleep(0.4);
            LCD.Clear();
            
            drivetrain.setExpectedSpeed(spe);
            drivetrain.setPIDConstants(p,i,d);
            if (forklift.front() == BP){
                drivetrain.Drive(dis);
            }
        }
    } while (!exitFlag);
}*/