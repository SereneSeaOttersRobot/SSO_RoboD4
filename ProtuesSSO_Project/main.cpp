#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>

//function prototypes
bool startLightWithinRange(float lightValue);

//global variables and defines
//Enumeration of Left, Right,...
#define RIGHT 2
#define LEFT 0
#define FRONT 1

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


//Returned datatype of detect light of lightsense
struct Color {
    #define ColorRED 1
    #define ColorBLUE 0 
    int color;
};

//cds sensor
AnalogInputPin sensor_temp_cds(FEHIO::P0_4);

class LightSense {
    public:
    AnalogInputPin cds = sensor_temp_cds;
    float blue_light_val = 0.433; //filter none, blue filter 0.525
    float red_light_val = 0.200; //filter none, blue filter 0.940
    LightSense();
    Color detectLight();
    bool detectRed();
}lumy;


//bumper sensor
DigitalInputPin sensor_bumper_left(FEHIO::P3_4);
DigitalInputPin sensor_bumper_front(FEHIO::P0_2);
DigitalInputPin sensor_bumper_right(FEHIO::P0_3);


/**
 * Main function of program
 */
int main(void) {
    //preload

    //manual ready up
    LCD.Clear(BLACK);
    LCD.WriteLine("Press front bumper to ready up");
    while(sensor_bumper_front.Value());

    do {
        LCD.Clear(BLACK);
        LCD.WriteLine("Bumper pressed, waiting on light");
        LCD.WriteLine("Light value detected : ");
        LCD.WriteLine(lumy.cds.Value());
        LCD.WriteLine("Not detected as red");
    } while(!lumy.detectRed());
    LCD.Clear(BLACK);
    LCD.WriteLine("Light value detected : ");
    LCD.WriteLine(lumy.cds.Value());
    LCD.WriteLine("Detected as red");
    LCD.WriteLine("Moving in 3 seconds");
    Sleep(3.0);

    LCD.Clear(BLACK);
    LCD.WriteLine("Turn Right, motors(25,-25) sleep(3)");
    motors.setPerc(25,-25);
    Sleep(3.0);

    LCD.Clear(BLACK);
    LCD.WriteLine("Entering Outer Main While loop, end condition is all buttons pressed at once");
    //clear update screen counter
    int iteration = 0;
    while (sensor_bumper_left.Value() || sensor_bumper_front.Value() || sensor_bumper_right.Value()) {
        //clear screen logic
        if (iteration % 7 == 1){
            LCD.Clear(BLACK);
            LCD.WriteLine("End codition is all buttons pressed at once");
            LCD.Write("Iteration (gaps) : ");
            LCD.WriteLine(iteration);
        }

        //base move forward line
        motors.setPerc(25,23);

        bool front, left, right;
        do {
            front = sensor_bumper_front.Value();
            left = sensor_bumper_left.Value();
            right = sensor_bumper_right.Value();
        } while(front && left && right);
        
        /*
        boolean values returned by bumpers are intuitively opposite.
        true when not pressed
        false when pressed
        */

        if (!front){
            LCD.WriteLine("Move Backwards motors(-25) sleep(2)");
            motors.setPerc(-25);
            Sleep(2);
        }

        if (!right){
            LCD.WriteLine("Turn slightly Left, motors(-25, 10) sleep (1)");
            motors.setPerc(-25,10);
            Sleep(1);
        }

        if (!left){
            LCD.WriteLine("Turn slightly Right, motors(10, -25) sleep (1)");
            motors.setPerc(10,-25);
            Sleep(1);
        }

        iteration++;
    }

}

bool startLightWithinRange(float lightValue){
    bool res = true; // for testing, change back to false after testing
    //chain of ifs, account for tolerances
    return res;
}

LightSense::LightSense(){
    //no argument constructor
};

bool LightSense::detectRed(){
    bool res = false;
    if (detectLight().color == ColorRED){
        res = true;
    }
    return res;
}

Color LightSense::detectLight(){
    Color res;
    float lightValue = cds.Value();
    const float tolerance = 0.01;
    if (lightValue >= blue_light_val - tolerance && lightValue <= blue_light_val+tolerance){
        res.color = ColorBLUE;
    } else if (lightValue >= red_light_val - tolerance && lightValue <= red_light_val + tolerance){
        res.color = ColorRED;
    }
    return res;
}