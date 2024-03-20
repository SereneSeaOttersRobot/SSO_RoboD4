#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#define POWER 25.0


int main(void)
{
    //AnalogInputPin cds(FEHIO::P0_0);
    /*
    FEHMotor lefDr(FEHMotor::Motor2, 9.0);
    FEHMotor rigDr(FEHMotor::Motor0, 9.0);
    DigitalInputPin microFL(FEHIO::P3_7);
    DigitalInputPin microFR(FEHIO::P0_2);
    DigitalInputPin microBL(FEHIO::P3_5);
    DigitalInputPin microBR(FEHIO::P0_4);
    */
    FEHMotor forklift(FEHMotor::Motor3, 5.0);
    DigitalInputPin microBottom(FEHIO::P3_5);
    DigitalInputPin microTop(FEHIO::P0_7);
    AnalogInputPin sensor_temp_cds(FEHIO::P0_0);
    float x,y;
    /*
    float lightValue = sensor_temp_cds.Value();
    x=0.4;
    y=0.1;

    while (lightValue>x || lightValue<y){
        lightValue=sensor_temp_cds.Value();
        LCD.WriteLine(lightValue);
        Sleep (0.1);
    }
    */
    float direction=90.;
    
    forklift.SetPercent(direction);
    while (1){
        if (microBottom.Value()==0){
            
            direction=-1*direction;
            forklift.SetPercent(direction);
            Sleep (0.2);
        } else if (microTop.Value()==0){
            
            direction=-1*direction;
            forklift.SetPercent(direction);
            Sleep (0.2);
        }
    }
    /*
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
    */
}
