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

/*
If moto test works then this is next test, making sure encoders work for reading stuff
*/

// void moveForward(double inches, FEHFile * file);

// Motors moto;
// Encoders cod;


// int main(){
//     LCD.Clear(BLACK);
//     LCD.WriteLine("Using single single 3.75 tick");
//     LCD.WriteLine("Going 5 inches on that");
//     Sleep(3.);
//     LCD.Clear();
//     LCD.WriteLine("Declaring and assigning variables");
//     //encoder
//     AnalogEncoder tempencleft(LEFT_ENCODER_PORT);
//     AnalogEncoder tempencright(RIGHT_ENCODER_PORT);
//     //cod = Encoders(tempencleft,tempencright); ---this doesn't work
//     cod.left = &tempencleft;
//     cod.right = &tempencright;
//     cod.setThresholds(LOW_THRESHOLD,HIGH_THRESHOLD);
//     //motors
//     FEHMotor tempmotleft(LEFT_MOTOR_PORT,VOLTAGE);
//     FEHMotor tempmotright(RIGHT_MOTOR_PORT,VOLTAGE);
//     //moto = Motors(tempmotleft,tempmotright); ---this doesn't work
//     moto.left = &tempmotleft;
//     moto.right = &tempmotright;
//     //is right motor going correct direction? (true or false)
//     bool rightMotorDir = false; //determine right motor is in wrong direction so false
//     //what about left motor?
//     bool leftMotorDir = true; //left motor works correctly
//     //have moto handle that then
//     moto.setMotorDirection(leftMotorDir,rightMotorDir);
//     Sleep(1.0);
//     LCD.Clear();
//     LCD.WriteLine("Starting in 5 seconds");
//     char filename[] = "encoder1.txt";
//     LCD.Write("Data is stored in ");
//     LCD.WriteLine(filename);
//     FEHFile * file = SD.FOpen(filename,"w");
//     Sleep(5.0);
//     LCD.Clear();
//     LCD.WriteLine("Moving forward 5 inches");
//     SD.FPrintf(file,"Moving forward 5 inches\n");
//     Sleep(0.5);
//     double firstMove = 5.0;
//     moveForward(firstMove, file);
//     //end of test for now
//     moto.stop();
//     SD.FCloseAll();
// }


// void moveForward(double inches, FEHFile * file){
//     double currentInches = 0.0;
//     //should make class function that reports the average of the two ticks
//     //display archive ticks, logic off current ticks
//     cod.resetTicks();
//     LCD.Clear();
//     LCD.WriteLine("Archive ticks and current inches will be displayed");
//     SD.FPrintf(file,"Archive ticks and current inches will be displayed\n");
//     moto.setPerc(15.0);
//     int lcdcount = 10;
//     do {
//         long alef, arig;
//         int lef,rig;
//         cod.archives(alef,arig);
//         cod.ticks(lef,rig);
//         currentInches = ((lef+rig)/2)*TICKS_INCHES;
//         if (lcdcount >= 10){
//             LCD.Clear();
//             LCD.Write("CurInches: ");
//             LCD.WriteLine(currentInches);
//             LCD.Write("lef: ");
//             LCD.Write(((int)alef));
//             LCD.Write(" rig: ");
//             LCD.WriteLine(((int)arig));
//             lcdcount = 0;
//         } else {
//             lcdcount++;
//         }
//         SD.FPrintf(file,"lef: %i \t| rig: %i \t| curInches: %0.5d",((int)alef),((int)arig),currentInches);
//     } while (currentInches < inches);
//     moto.stop();
// }







// /*
// Below is test for global motors class but FEHMotors are declared inside the main and set into the global variable.
// */
// void useMoto();

// Motors moto; //default constructed

// int main(){
//     //call useMoto, motors should move if this works
//     LCD.Clear();
//     LCD.WriteLine("Staring");
//     FEHMotor tempLef(LEFT_MOTOR_PORT,VOLTAGE);
//     FEHMotor tempRig(RIGHT_MOTOR_PORT,VOLTAGE);
//     moto.left = &tempLef;
//     moto.right = &tempRig;
//     useMoto();
//     LCD.WriteLine("After useMoto");
// }

// void useMoto(){
//     moto.setPerc(15.0);
//     Sleep(3.0);
//     moto.stop();
// }

/*
 Results for motors declarations
 Passing the objects through a function will not work at all
 FEHMotor objects can be declared globaly and then have moto point to them in main.*
 FEHMotor objects can be declared in main then have moto point to them in main.*
 *This is moto.left = &object and moto.right = &object, remember can't use function to pass these two.

 Right motor is in reversed direction, left is in correct forward direction (as of 3/5/2024)
*/

/*
Testing servo
servo works
*/

// int main(){
//     FEHMotor servo(FEHMotor::Motor0, 5.0);
//     LCD.Clear();
//     LCD.WriteLine("Starting, for 1 sec");
//     servo.SetPercent(50.0);
//     Sleep(5.0);
//     LCD.WriteLine("Finish");
//     servo.Stop();
// }

// LightSensor lumy;

// int main(){
//     AnalogInputPin templumy(FEHIO::P1_0);
//     lumy.cds_sensor = &templumy;
//     lumy.setTolerance(0.15);
//     lumy.setLights(Base_Red_Light,Base_Blue_Light);
//     LCD.Clear(BLACK);
//     LCD.Write("Start detect blue: ");
//     LCD.WriteLine(Base_Blue_Light);
//     Sleep(0.4);
//     while (true){
//         LCD.Clear();
//         LCD.Write("Lit val: ");
//         LCD.WriteLine(lumy.lightValue());
//         LCD.Write("Color Detected? ");
//         LightSensor::LColor color = lumy.detectColor();
//         if (color == LightSensor::LRED){
//             LCD.WriteLine("Yes, Red");
//         } else if (color == LightSensor::LBLUE){
//             LCD.WriteLine("Yes, Blue");
//         } else {
//             LCD.WriteLine("No, empty");
//         }
//         Sleep(0.3);
//     }
// }

int main(){
    AnalogEncoder testEncoder(FEHIO::P0_0);
    AnalogEncoder testEncoder2(FEHIO::P0_2);
    LCD.Clear();
    LCD.WriteLine("Starting Encoder Test");
    testEncoder.SetThresholds(0.15,2.35);
    testEncoder.ResetCounts();
    testEncoder2.SetThresholds(0.15,2.35);
    // testEncoder2.ResetCounts();
    Sleep(0.5);
    while(true){
        LCD.WriteLine(testEncoder.Counts());
        LCD.WriteLine(testEncoder2.Counts());
        Sleep(0.2);
        LCD.Clear();
    }
}