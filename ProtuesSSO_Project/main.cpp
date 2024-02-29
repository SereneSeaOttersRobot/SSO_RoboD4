#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHMotor.h>
#include <FEHAccel.h>
#include <FEHSD.h>
/*
FEHMotor leftVex(FEHMotor::Motor1, 1.0);
FEHMotor rightVex(FEHMotor::Motor2, 1.0);
AnalogInputPin cds1(FEHIO::P0_0);
FEHServo liftServo(FEHServo::FEHServoPort);
AnalogInputPin optoLine1(FEHIO::P0_1);
AnalogInputPin optoLine2(FEHIO::P0_1);
AnalogInputPin optoLine3(FEHIO::P0_1);
*/

    FEHMotor lefDr(FEHMotor::Motor0, 9.0);
    FEHMotor rigDr(FEHMotor::Motor2, 9.0);
    DigitalInputPin microFL(FEHIO::P2_7);
    DigitalInputPin microFR(FEHIO::P0_0);
    DigitalInputPin microF(FEHIO::P2_0);
    AnalogInputPin sensor_temp_cds(FEHIO::P1_0);


int main(void)
{
    
    //AnalogInputPin cds(FEHIO::P0_0);


    //DigitalInputPin microBR(FEHIO::P0_4);
    float x, y, i;
    /*
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

    //motor practice
    /*
    rigDr.SetPercent(50.);
    Sleep (10000);
    rigDr.Stop();
    rigDr.SetPercent(-50.);
    Sleep(10000);
    rigDr.Stop();

    lefDr.SetPercent(50.);
    Sleep (10000);
    lefDr.Stop();
    lefDr.SetPercent(-50.);
    Sleep(10000);
    lefDr.Stop();
    */
   
   //no light over light, 3.02
    // red light 0.295
    //blue light 0.94
    x=0.4;
    y=0.1;
    i=0;
   // lefDr.SetPercent(50.); 
   // rigDr.SetPercent(-50.); 
  float lightValue = sensor_temp_cds.Value();

    while (lightValue>x || lightValue<y){
        lightValue=sensor_temp_cds.Value();
        LCD.WriteLine(lightValue);
        Sleep (0.1);
    }
    
    LCD.WriteLine("Red found");
    //initial spin right
    lefDr.SetPercent(25.); 
    rigDr.SetPercent(25.); 
    Sleep(0.5);
    lefDr.Stop();
    rigDr.Stop();

    //turn right and run into first walll
    lefDr.SetPercent(35.); 
    rigDr.SetPercent(-25.); 
    while (microFR.Value()==1 && microF.Value()==1){}    //wait until it hits a wall
    LCD.WriteLine("Hit first wall");
    lefDr.Stop();
    rigDr.Stop();
    Sleep(0.25);

    int r=1,c=0,l=0;
    while (1){
        if (r==1){
            //spin left
            lefDr.SetPercent(-25.);
            rigDr.SetPercent(-25.); 
            Sleep(0.25);
            lefDr.Stop();
            rigDr.Stop();
            LCD.WriteLine("Left spin complete");
        }
        if (c==1){
            //probably has hit passport, back up and go left then spin straight
            i=i+1; 
            if (i==1){
                //going back and left
                lefDr.SetPercent(-25.); 
                rigDr.SetPercent(60.);
                Sleep (1.5);
                lefDr.Stop();
                rigDr.Stop();

                //spinning left
                lefDr.SetPercent(-25.); 
                rigDr.SetPercent(-25.); 
                Sleep(2.0);
                lefDr.Stop();
                rigDr.Stop();
                LCD.WriteLine("backup and spin left complete");
            }
            if (i==2){
                //going back and left
                lefDr.SetPercent(-25.); 
                rigDr.SetPercent(60.);
                Sleep (1.5);
                lefDr.Stop();
                rigDr.Stop();

                //spinning right
                lefDr.SetPercent(25.); 
                rigDr.SetPercent(25.); 
                Sleep(0.85);
                lefDr.Stop();
                rigDr.Stop();
                LCD.WriteLine("backup and spin right complete");
            }
            if (i>2){
                //spin right
                lefDr.SetPercent(25.);
                rigDr.SetPercent(25.); 
                Sleep(0.35);
                lefDr.Stop();
                rigDr.Stop();
                LCD.WriteLine("right spin complete");
            }

        }

        if (l==1){
            //spin right
            lefDr.SetPercent(25.);
            rigDr.SetPercent(25.); 
            Sleep(0.35);
            lefDr.Stop();
            rigDr.Stop();
            LCD.WriteLine("right spin complete");
        }
        //going straight and slightly right
        if (i<2){
            lefDr.SetPercent(30.); 
            rigDr.SetPercent(-22.);
        }
        //going straight and slightly left
        if (i>=2){
            lefDr.SetPercent(22.); 
            rigDr.SetPercent(-23.);
        }
        
        while (microFR.Value()==1 && microF.Value()==1 &&microFL.Value()==1){
            
            if (microFR.Value()==0){
                r=1;
                c=0;
                l=0;
            }
            if (microF.Value()==0){
                c=1;
                r=0;
                l=0;
            }
            if (microFL.Value()==0){
                c=0;
                r=0;
                l=1;
            }
        
            
        }
        LCD.WriteLine("Hit wall");
        lefDr.Stop();
        rigDr.Stop();

    } 
   /*
   while (1){
    if (microFL.Value()==0){
        LCD.WriteLine("Left bumper pressed");
    }
    if (microFR.Value()==0){
        LCD.WriteLine("Right bumper pressed");
    }
    if (microF.Value()==0){
        LCD.WriteLine("Front bumper pressed");
    }
    float lightValue = sensor_temp_cds.Value();
    LCD.WriteLine(lightValue);
    Sleep (0.5);
    }
    */

   //lefDr.Stop();
   //rigDr.Stop();
}
