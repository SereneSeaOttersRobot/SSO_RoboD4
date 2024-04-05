#include <Constants.h>
#include <CdSensor.h>
#include <LineFollower.h>
#include <ForkLift.h>
//#include <FEHIO.h>

CdSensor CdS(CdS_Port);
LineFollower lf_left(LF_Left_Port, LEFT);
LineFollower lf_middle(LF_Middle_Port, MIDDLE);
LineFollower lf_right(LF_Right_Port, RIGHT);
ForkLift forklift(Motor_Forklift_Port, Forklift_Voltage);

int main() {
    //setup forklift buttons
    forklift.setButtonPins(Button_ForkTop_Port,Button_ForkBot_Port,Button_ForkFront_Port);
    

    //wait on red
    while(CdS.notRed());

    //wait on blue
    while(CdS.notBlue());

    //wait for viable light, report that light
    bool red = false, blue = false;
    do {
        red = !CdS.notRed();
        blue = !CdS.notBlue();
    } while (!(red || blue));

    //detect white line for each lf
    bool huh = lf_left.onWhite();
    bool huh2 = lf_middle.onWhite();
    bool huh3 = lf_right.onWhite();
    
    //detect yellow line for each lf
    bool ugh = lf_left.onYellow();
    bool ugh2 = lf_middle.onYellow();
    bool ugh3 = lf_right.onYellow();

    /*
    ///////////////////STEP 7 Moving to passport and completing passport/////////////////////////////
    //turn towards line
    Turn(120.0,TURNSPEED,LEFT);
    Drive(13.,FASTSPEED);

    //turning left until line is found
    leftMotor.SetPercent(-15.);
    rightMotor.SetPercent(15.);
    while (!lf_middle.onWhite()); //while middle has not detected line
    leftMotor.Stop();
    rightMotor.Stop();
    //following the line
    LineFollow(color);  //stops one if doesnt find the line

    


    void StampArm() {
    /////////////////// Moving to and under Stamp Arm ///////////////////

    //move forward to setup for turn
    moveForward(2.0,15.0);
    //turn to stamp arm
    turnRight(80.0,15.0);

    ////////////////// Lifting and Pushing Stamp Arm //////////////////////

    //forklift up for a few seconds
    forklift.SetPercent(-percent); //currently percent = -90.0
    Sleep(2.0);
    forklift.Stop();

    //move forward 1.15 inches, to get further under stamp arm
    moveForward(1.15, 15.0);

    //turn a little left so to stay under stamp arm when near top of forklift height.
    turnLeft(5.0,15.0);

    //forklift go up for two seconds
    forklift.SetPercent(-percent);
    Sleep(2.0);
    forklift.Stop();
    
    // Turn slightly and go forward more to get more under
    turnLeft(5.0,15.0);
    moveForward(0.5,15.0);
    
    // Raise forklift all the way up
    forklift.SetPercent(-percent);
    while(forktop.Value() != BP){
    }
    forklift.Stop();

    //turn left to push stamp arm
    turnLeft(25.0,30.0);
    //Move forward to push stamp arm more
    moveForward(.75,50.0);

    ////////////////// Getting Stamp Arm Back Down /////////////////


    
    //move back to get out of the way of the stamp arm
    moveBackward(2,30.0);

    // turn to get to left of stamp
    turnLeft(30.0,30);

    //Drive forwards to get back in plane with arm
    moveForward(3.5,30.0);

    //turn right to hit stamp down
    turnRight(50.0,30);

    // Move back to let it fall
    moveBackward(5.0,30.0);
    }

    void moveForward(double inches, float percent)
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
    int ticks = (int)(inches * TICKS_PER_INCH / 1.22);
    leftMotor.SetPercent(percent + 0.75);
    rightMotor.SetPercent(percent);
    while (((leftEncoder.Counts() + rightEncoder.Counts()))/2  < ticks)
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
    */

}
