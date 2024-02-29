#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>


//Motor voltage
#define MOTOR_VOLT 9.0
FEHMotor tempLeft(FEHMotor::Motor0, MOTOR_VOLT);
FEHMotor tempRight(FEHMotor::Motor2, MOTOR_VOLT);
//motors, methods for handling both at same time.
class Motor {
    public:
    FEHMotor left = tempLeft;
    FEHMotor right = tempRight;
    Motor(){};
    void setPerc(float percent){
        left.SetPercent(percent);
        right.SetPercent(percent);
    }
    void setPerc(float leftP, float rightP){
        left.SetPercent(leftP);
        right.SetPercent(rightP);
    }

    void stop(){
        left.Stop();
        right.Stop();
    }
}motors;

//encoder part
#define Low_Threshold 0.15
#define High_Threshold 2.25

AnalogEncoder enR(FEHIO::P0_5);
AnalogEncoder enL(FEHIO::P2_4);

#define TICK_INCH 0.07853982
#define EPSILON 0.00001

#define LONG_TOUCH 1000

/**
 * Main function of program
 */
int main(void) {
    LCD.Clear(BLACK);
    LCD.WriteLine("Setting thresholds");
    LCD.Write("Low : ");
    LCD.WriteLine(Low_Threshold);
    LCD.Write("High : ");
    LCD.WriteLine(High_Threshold);
    enR.SetThresholds(Low_Threshold, High_Threshold);
    enL.SetThresholds(Low_Threshold, High_Threshold);
    Sleep(2.0);

    //exit
    //linear
    //-increase / decrease inches
    //-set in motion
    //--displays ticks and determined inches
    //--stop button
    //-back
    //turn
    //-increase / decrease angle
    //-set in motion
    //--displays ticks and determined angle
    //--stop
    //-back
    int exit = 0;
    FEHIcon::Icon exLiTu_buttons[3];
    char labelelt[3][20] = {"Exit","Linear","Turn"};
    FEHIcon::Icon semBack_buttons[2];
    char labelsemBack[2][20] = {"Set Go", "Back"};
    float x, y;
    FEHIcon::DrawIconArray(exLiTu_buttons,3,1,20,20,60,60,labelelt,WHITE,WHITE);
    while (exit == 0){
        LCD.ClearBuffer();
        while(!LCD.Touch(&x,&y));
        while(LCD.Touch(&x,&y));
        if (exLiTu_buttons[0].Pressed(x,y,1)){
            exit = 1;
            continue;
            //jumps icon array
        } else if (exLiTu_buttons[1].Pressed(x,y,1)){
            LCD.Clear();
            FEHIcon::Icon numIn_but[2];
            char labnumIn[2][20] = {"num","inches"};
            FEHIcon::Icon incDec_but[2];
            char labincDec[2][20] = {"incr 0.1", "decre 0.1"};
            double setInches = 0.0;
            bool back = false;
            FEHIcon::DrawIconArray(numIn_but,1,2,10,209,40,40,labnumIn,WHITE,WHITE);
            FEHIcon::DrawIconArray(incDec_but,1,2,40, 179, 40, 40, labincDec,WHITE,WHITE);
            FEHIcon::DrawIconArray(semBack_buttons,1,2,70,149,40,40,labelsemBack,WHITE,WHITE);
            while (!back){
                float displaySI = (float) setInches;
                numIn_but[0].ChangeLabelFloat(displaySI);
                LCD.ClearBuffer();
                while(!LCD.Touch(&x,&y));
                int tC = 0;
                while(LCD.Touch(&x,&y) && tC <= LONG_TOUCH){
                    tC++;
                }
                if (semBack_buttons[1].Pressed(x,y,1)){
                    back = true;
                    continue;
                }else if(semBack_buttons[0].Pressed(x,y,1)){
                    //more complex stuff, dont clear screen
                    //LCD.Clear();
                    //reset encoder ticks
                    enR.ResetCounts();
                    enL.ResetCounts();
                    int tickL = 0, tickR = 0;
                    double currentInches = 0.0;
                    //stop once to stop motors, stop twice to exit
                    int stop = 0;
                    //change labels on buttons to enact plan of world dom...
                    numIn_but[1].ChangeLabelFloat(currentInches);
                    incDec_but[0].ChangeLabelInt(tickL);
                    incDec_but[1].ChangeLabelInt(tickR);
                    //change set go to go and back to stop
                    semBack_buttons[0].ChangeLabelString("Go");
                    semBack_buttons[1].ChangeLabelString("Stop");
                    //set motors to 15 percent each
                    motors.setPerc(15);
                    while (stop <2){
                        Sleep(0.1); //Sleep for a little bit so screen updates aren't blinding fast
                        tickL = enL.Counts();
                        tickR = enR.Counts();
                        //using left encoder for determining motion
                        currentInches = tickL*TICK_INCH;
                        //update screen
                        numIn_but[1].ChangeLabelFloat(currentInches);
                        incDec_but[0].ChangeLabelInt(tickL);
                        incDec_but[1].ChangeLabelInt(tickR);
                        //clear touch buffer
                        LCD.ClearBuffer();
                        //touch counter, stops program from not cycling while touched after a number of iterations.
                        int touchCounter = 0;
                        //boolean determining if this has been touched for touch ifs
                        bool touched = false;
                        //determines touch
                        while (LCD.Touch(&x,&y) && touchCounter <= LONG_TOUCH){
                            touchCounter++;
                        }
                        //instead of resign touched multiple times inside while for possibly up to 1000 iterations...
                        touched = touchCounter > 0; //if touched then touchCounter should be greater than 0 for this iteration.
                        //ERROR NOTE: possible error where non touch x,y values still trigger these ifs - FIXED - REQ REVIEW
                        //chain logic between get robot going again and robot hitting setInches.
                        if (currentInches+EPSILON>=setInches){
                            //robot stops if currentInches + tolerance is >= setInches
                            motors.stop();
                            //stop is set to 1 for robot has stop
                            stop = 1;
                        } else if (touched && semBack_buttons[0].Pressed(x,y,1)){
                            //if robot hasn't reached setInches, then this allowed to be checked
                            //if go button is pressed (touched and x,y in range), then robot moves again and stop is set to 0 (moving)
                            motors.stop(); //just in case setPercent is additive or something.
                            motors.setPerc(15);
                            stop = 0;
                        }
                        //button stop is not part of the logic chain as stop is also back.
                        //if stop is pressed, then robot is stopped and stop is incremented. If stop was 1, it becomes 2, exiting Set Go
                        //Or if stop is 0, then it becomes 1 and this stays in Set Go but doesn't move.
                        if (touched && semBack_buttons[1].Pressed(x,y,1)){
                            motors.stop();
                            stop++;
                        }
                    }
                    //end of Set Go, goodbye
                    //just in case
                    motors.stop();
                } else if (incDec_but[0].Pressed(x,y,1)){
                    setInches += 0.1;
                    continue;
                } else if (incDec_but[1].Pressed(x,y,1)){
                    setInches -= 0.1;
                    continue;
                }
                LCD.Clear();
                FEHIcon::DrawIconArray(numIn_but,1,2,10,209,40,40,labnumIn,WHITE,WHITE);
                FEHIcon::DrawIconArray(incDec_but,1,2,40, 179, 40, 40, labincDec,WHITE,WHITE);
                FEHIcon::DrawIconArray(semBack_buttons,1,2,70,149,40,40,labelsemBack,WHITE,WHITE);
            }
        } else if (exLiTu_buttons[2].Pressed(x,y,1)){
            LCD.Clear();
            FEHIcon::Icon numIn_but[2];
            char labnumIn[2][20] = {"num","degrees"};
            FEHIcon::Icon incDec_but[2];
            char labincDec[2][20] = {"incr 1", "decre 1"};
            double setDegrees = 0.0;
            bool back = false;
            FEHIcon::DrawIconArray(numIn_but,1,2,10,209,40,40,labnumIn,WHITE,WHITE);
            FEHIcon::DrawIconArray(incDec_but,1,2,40, 179, 40, 40, labincDec,WHITE,WHITE);
            FEHIcon::DrawIconArray(semBack_buttons,1,2,70,149,40,40,labelsemBack,WHITE,WHITE);
            while (!back){
                numIn_but[0].ChangeLabelFloat(setDegrees);
                LCD.ClearBuffer();
                while(!LCD.Touch(&x,&y));
                int tC = 0;
                while(LCD.Touch(&x,&y) && tC <= LONG_TOUCH){
                    tC++;
                }
                if (semBack_buttons[1].Pressed(x,y,1)){
                    back = true;
                    continue;
                } else if (semBack_buttons[0].Pressed(x,y,1)){
                    //stuff for degree when set Go
                    //reset encoders
                    enL.ResetCounts();
                    enR.ResetCounts();
                    //declare tick variables;
                    int tickL = 0, tickR = 0;
                    //declare currentDegrees and set to 0
                    double currentDegrees = 0.0;
                    //change screen to reflect
                    //setDegrees, currentDegrees
                    numIn_but[1].ChangeLabelFloat(currentDegrees);
                    //left ticks, right ticks
                    incDec_but[0].ChangeLabelInt(tickL);
                    incDec_but[1].ChangeLabelInt(tickR);
                    //  go      , stop(/back)
                    semBack_buttons[0].ChangeLabelString("go");
                    semBack_buttons[1].ChangeLabelString("Stop");
                    //stop, 0 - moving, 1 - not moving remain in loop, 2 - shouldn't be moving exit loop
                    int stop = 0;
                    //motors into motion, offset by 15 in opposite directions.
                    bool negDeg = false;
                    if (setDegrees < 0.0){
                        motors.setPerc(-15,15);
                        negDeg = true;
                    } else {
                        motors.setPerc(15,-15);
                    }
                    while (stop < 2){
                        Sleep(0.1); //Sleep for a little bit so screen updates aren't blinding fast
                        //read encoders
                        tickL = enL.Counts();
                        tickR = enR.Counts();
                        //current Degree math
                        //placeholder********************************** FLAG - Determining current degree is not done, place holder just goes 0.1 to currentDegrees
                        if (negDeg){
                            currentDegrees -= 0.1;
                        } else {
                            currentDegrees += 0.1;
                        }
                        //update display
                        numIn_but[1].ChangeLabelFloat(currentDegrees);
                        incDec_but[0].ChangeLabelInt(tickL);
                        incDec_but[1].ChangeLabelInt(tickR);
                        //determine touch ------
                        //touch counter to end long touched
                        int touchCounter = 0;
                        LCD.ClearBuffer();
                        while (LCD.Touch(&x,&y) && touchCounter <= LONG_TOUCH){
                            touchCounter++;
                        }
                        //determined if actually touched for x,y values
                        bool touched = touchCounter > 0; //if touchCounter is > 0 then this has been touched this iteration.
                        //logic chain for reaching setDegrees and blocking hitting go, or hitting go.
                        //for this with negDeg
                        bool curCod;
                        if (negDeg){
                            curCod = currentDegrees-EPSILON<=setDegrees;
                        }else{
                            curCod = currentDegrees+EPSILON>=setDegrees;
                        }
                        if (curCod){
                            //set motors to stop
                            motors.stop();
                            //stop = 1, for this has stopped moving
                            stop = 1;
                        } else if (touched && semBack_buttons[0].Pressed(x,y,1)){
                            //set motors to stop incase of additive for setpercents
                            motors.stop();
                            //set motors following negDeg
                            if (negDeg){
                                motors.setPerc(-15,15);
                            } else {
                                motors.setPerc(15,-15);
                            }
                            //stop is reset to 0 for moving
                            stop = 0;
                        }
                        //reaching currentDegree shall not block stop button as stop button is also back.
                        if (touched && semBack_buttons[1].Pressed(x,y,1)){
                            //stop this
                            motors.stop();
                            //increment stop
                            stop++;
                        }
                    }
                    //end of set go, goodbye
                    motors.stop(); //just in case
                } else if (incDec_but[0].Pressed(x,y,1)){
                    setDegrees += 1;
                    continue;
                } else if (incDec_but[1].Pressed(x,y,1)){
                    setDegrees -= 1;
                    continue;
                }
                LCD.Clear();
                FEHIcon::DrawIconArray(numIn_but,1,2,10,209,40,40,labnumIn,WHITE,WHITE);
                FEHIcon::DrawIconArray(incDec_but,1,2,40, 179, 40, 40, labincDec,WHITE,WHITE);
                FEHIcon::DrawIconArray(semBack_buttons,1,2,70,149,40,40,labelsemBack,WHITE,WHITE);
            }
        } else {
            continue;
            //jumps draw icon array
        }
        LCD.Clear();
        FEHIcon::DrawIconArray(exLiTu_buttons,3,1,20,20,60,60,labelelt,WHITE,WHITE);
    }


}