#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>

FEHMotor leftVex(FEHMotor::Motor1, 1.0);
FEHMotor rightVex(FEHMotor::Motor2, 1.0);
AnalogInputPin cds1(FEHIO::P0_0);
FEHServo liftServo(FEHServo::FEHServoPort);
AnalogInputPin optoLine1(FEHIO::P0_1);
AnalogInputPin optoLine2(FEHIO::P0_1);
AnalogInputPin optoLine3(FEHIO::P0_1);




int main(void)
{
    //AnalogInputPin cds(FEHIO::P0_0);
    FEHMotor lefDr(FEHMotor::Motor2, 9.0);
    FEHMotor rigDr(FEHMotor::Motor0, 9.0);
    DigitalInputPin microFL(FEHIO::P3_7);
    DigitalInputPin microFR(FEHIO::P0_2);
    DigitalInputPin microBL(FEHIO::P3_5);
    DigitalInputPin microBR(FEHIO::P0_4);
    float x, y;
    LCD.ClearBuffer();
    while(!LCD.Touch(&x,&y));
    while(LCD.Touch(&x,&y));
    //Part 1
    lefDr.SetPercent(24.);
    rigDr.SetPercent(25.);
    while(microFL.Value() || microFR.Value());
    lefDr.Stop();
    rigDr.Stop();
    //Part 2
    rigDr.SetPercent(-30.);
    lefDr.SetPercent(5.);
    while(microBL.Value() || microBR.Value());
    lefDr.Stop();
    rigDr.Stop();
    //Part 3
    lefDr.SetPercent(24.);
    rigDr.SetPercent(25.);
    while(microFL.Value() || microFR.Value());
    lefDr.Stop();
    rigDr.Stop();
    //Part 4..
    lefDr.SetPercent(-30.);
    rigDr.SetPercent(5.);
    while(microBL.Value() || microBR.Value());
    lefDr.Stop();
    rigDr.Stop();
    //Part 5
    lefDr.SetPercent(24.);
    rigDr.SetPercent(25.);
    while(microFL.Value() || microFR.Value());
    lefDr.Stop();
    rigDr.Stop();    
    
}
