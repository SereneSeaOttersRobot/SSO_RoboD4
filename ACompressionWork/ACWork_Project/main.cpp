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


/*
Function prototypes
*/



void DriveTrainConstantTesting();

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
    drivetrain.setEncoders(leftEncoder, rightEncoder); //should change name to assign... as thats what it is actually doing.

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

    LCD.WriteLine("Trying inside driveTrain");
    drivetrain.testDriveRight();
    /*
    If this doesn' work ideas to fix
    set encoders outside of constructor for setup for assignment operator
    add another port for null, discrimating the two encoders from each other on creation
    ask TA
    globalize the encoders?
    set encoders as reference into needed functions
    transfer the function over to here

    */
    

    LCD.WriteLine("Test is over");
    return 1;
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