#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHIO.h>

FEHServo servo(FEHServo::Servo7); // declare servo arm
int main(void)
{
    servo.SetMax(2262);
    servo.SetMin(890);
    AnalogInputPin cds(FEHIO::P0_0);
    while(1){
        float angle = cds.Value() * 54.54;
        servo.SetDegree(angle);
        LCD.Clear(BLACK);
        LCD.WriteLine(angle);
        Sleep(0.2);
    }
    
    
}
