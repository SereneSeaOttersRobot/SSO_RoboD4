#include <LineFollower.h>
#include <Constants.h>
#include <FEHIO.h>

LineFollower::LineFollower(FEHIO::FEHIOPin pin, Side side):AnalogInputPin(pin){
    this->thisSide = side;
}

bool LineFollower::onWhite(){
    bool result = false;
    int value = Value();
    //switch to side that this is designated as and use those values.
    switch(thisSide){
        case LEFT:{
            result = (LF_Left_White_Lower <= value && value <= LF_Left_White_Upper);
            break;
        }
        case MIDDLE:{
            result = (LF_Middle_White_Lower <= value && value <= LF_Middle_White_Upper);
            break;
        }
        case RIGHT:{
            result = (LF_Right_White_Lower <= value && value <= LF_Right_White_Upper);
            break;
        }
        default: {
            //Should never be used.
        }
    }
    return result;
}

bool LineFollower::onYellow(){
    bool result = false;
    int value = Value();
    //switch to side that this is designated as and use those values.
    switch(thisSide){
        case LEFT:{
            result = (LF_Left_Yellow_Lower <= value && value <= LF_Left_Yellow_Upper);
            break;
        }
        case MIDDLE:{
            result = (LF_Middle_Yellow_Lower <= value && value <= LF_Middle_Yellow_Upper);
            break;
        }
        case RIGHT:{
            result = (LF_Right_Yellow_Lower <= value && value <= LF_Right_Yellow_Upper);
            break;
        }
        default: {
            //Should never be used.
        }
    }
    return result;
}