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
    // RCS.InitializeTouchMenu(Team_Key);
    // const int Lever = RCS.GetCorrectLever(); //follows the side enumeration

    

    LCD.Clear();
    LCD.WriteLine("Starting Test");
    Sleep(2.0);

    
    while (forklift.front() == BNP){
        LCD.Write("Right: ");
        LCD.WriteLine(rightEncoder.Counts());
        Sleep(0.1);
        if (forklift.top() == BP){
            rightMotor.resetin(rightEncoder);
        }
    }

    /*
    If this doesn' work ideas to fix
    set encoders outside of constructor for setup for assignment operator
    add another port for null, discrimating the two encoders from each other on creation
    ask TA
    globalize the encoders?
    set encoders as reference into needed functions
    transfer the function over to here

    */

    LCD.WriteLine("Starting PID testing");
    Sleep(1.5);
    PIDTesting();
    

    LCD.WriteLine("Test is over");
    return 1;
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
        Sleep(0.1);
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