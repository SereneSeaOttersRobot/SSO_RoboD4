#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHSD.h>
#include <Motors.h>

// Declarations for analog optosensors
AnalogInputPin right_opto(FEHIO::P1_0);
AnalogInputPin middle_opto(FEHIO::P1_3);
AnalogInputPin left_opto(FEHIO::P1_6);

FEHMotor leftTemp(FEHMotor::Motor1,9.0);
FEHMotor rightTemp(FEHMotor::Motor0,9.0);
Motors motors(FEHMotor::Motor0,FEHMotor::Motor1,9.0);


//Defines for 'yes' values with opto_sensors
#define LefYes 1.9
#define RigYes 2.26
#define MidYes 2.37
#define tolerance 0.2

//enum line states
enum LineStates {
    MIDDLE = 0,
    RIGHT = 1,
    LEFT = 2
};


bool Equ(float x, float y);

int main(void)
{
    //preload motors class
    motors.left = &leftTemp;
    motors.right = &rightTemp;
    float x, y; //for touch screen

    // Open output file and prepare for writing values to it
    // <ADD CODE HERE>
    FEHFile * file = SD.FOpen("LFAc.txt","w");

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Analog Optosensor Testing");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    //middle yes drive straight
    //left yes drive right
    //right yes drive left
    //both middle and left yes slight right
    //both middle and right yes slight left
    int state = MIDDLE;
    SD.FPrintf(file,"\nMIDDLE");
    SD.FPrintf(file,"\nLeft Opto Sensor : %f",left_opto.Value());
    SD.FPrintf(file,"\nMiddle Opto Sensor : %f",middle_opto.Value());
    SD.FPrintf(file,"\nRight Opto Sensor : %f\n",right_opto.Value());
    while (true) {
        switch(state) {
            case MIDDLE:
                LCD.WriteLine("MIDDLE");
                SD.FPrintf(file,"\nMIDDLE");
                motors.setPerc(15.0,-15.0);
                if (Equ(right_opto.Value(),RigYes)){
                    state = RIGHT;
                }
                if (Equ(left_opto.Value(),LefYes)){
                    state = LEFT;
                }
                break;
            case LEFT:
                LCD.WriteLine("LEFT");
                SD.FPrintf(file,"\nLEFT");
                motors.setPerc(5.0,-15.0);
               
                    if (Equ(right_opto.Value(),RigYes)){
                        state = RIGHT;
                    }
                    if (Equ(middle_opto.Value(),MidYes)){
                        state = MIDDLE;
                    }
                
                break;
            case RIGHT:
                LCD.WriteLine("RIGHT");
                SD.FPrintf(file,"\nRIGHT");
                motors.setPerc(15.0,-5.0);
                
                    if (Equ(left_opto.Value(),LefYes)){
                        state = LEFT;
                    }
                    if (Equ(middle_opto.Value(),MidYes)){
                        state = MIDDLE;
                    }
                break;
            default:
                LCD.WriteLine("ERROR: NOTHING FOUND");
                break;
        }
        SD.FPrintf(file,"\nLeft Opto Sensor : %f",left_opto.Value());
        SD.FPrintf(file,"\nMiddle Opto Sensor : %f",middle_opto.Value());
        SD.FPrintf(file,"\nRight Opto Sensor : %f\n",right_opto.Value());
        Sleep(0.1);
    }


    // Close output file
    // <ADD CODE HERE> 
    SD.FCloseAll();
    // Print end message to screen
    LCD.Clear(BLACK);
    LCD.WriteLine("Test Finished");

    return 0;
}

bool Equ(float x, float y){
    bool res = false;
    //don't need to wait
    if (x >= y-tolerance && x<= y+tolerance){
        res = true;
    }

    return res;
}