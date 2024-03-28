#include <Constants.h>
#include <CdSensor.h>
#include <LineFollower.h>
#include <ForkLift.h>
//#include <FEHIO.h>

CdSensor CdS(CdS_Port);
LineFollower lf_left(LF_Left_Port, LEFT);
LineFollower lf_middle(LF_Middle_Port, MIDDLE);
LineFollower lf_right(LF_Right_Port, RIGHT);
ForkLift forklift(Motor_Forklift_Port, Forklift_Voltage);

int main() {
    //setup forklift buttons
    forklift.setButtonPins(Button_ForkTop_Port,Button_ForkBot_Port,Button_ForkFront_Port);
    

    //wait on red
    while(CdS.notRed());

    //wait on blue
    while(CdS.notBlue());

    //wait for viable light, report that light
    bool red = false, blue = false;
    do {
        red = !CdS.notRed();
        blue = !CdS.notBlue();
    } while (!(red || blue));

    //detect white line for each lf
    bool huh = lf_left.onWhite();
    bool huh2 = lf_middle.onWhite();
    bool huh3 = lf_right.onWhite();
    
    //detect yellow line for each lf
    bool ugh = lf_left.onYellow();
    bool ugh2 = lf_middle.onYellow();
    bool ugh3 = lf_right.onYellow();

}
