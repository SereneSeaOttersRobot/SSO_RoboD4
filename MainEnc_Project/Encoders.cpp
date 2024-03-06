#include <Encoders.h>
#include <FEHProteus.h>
#include <fehproteusfirmware\Libraries\FEHIO.h>

Encoders::Encoders(){
    reverseDirLeft = false;
    reverseDirRight = false;
    archive_left = 0l;
    archive_right = 0l;
    left = nullptr;
    right = nullptr;
    leftDouble = nullptr;
    rightDouble = nullptr;
    doubleSignal = false;
}
/** @deprecated */
Encoders::Encoders(AnalogEncoder &lef, AnalogEncoder &rig){
    left = &lef;
    right = &rig;
    reverseDirLeft = false;
    reverseDirRight = false;
    archive_left = 0l;
    archive_right = 0l;
}

Encoders::~Encoders(){
    left = nullptr;
    right = nullptr;
}

void Encoders::archives(long &lef, long &rig){
    int le, ri;
    ticks(le,ri);
    lef = archive_left + le;
    rig = archive_right + ri;
}

void Encoders::setThresholds(float low, float high){
    left->SetThresholds(low, high);
    right->SetThresholds(low, high);
    if (doubleSignal){
        leftDouble->SetThresholds(low,high);
        rightDouble->SetThresholds(low,high);
    }
}

void Encoders::resetTicks(){
    int le, ri;
    ticks(le,ri);
    archive_left += le;
    archive_right += ri;
    left->ResetCounts();
    right->ResetCounts();
    if (doubleSignal){
        leftDouble->ResetCounts();
        rightDouble->ResetCounts();
    }
}
/** @deprecated */
void Encoders::setEncoders(AnalogEncoder &lef, AnalogEncoder &rig){
    left = &lef;
    right = &rig;
}

void Encoders::setDir(bool lefDir, bool rigDir){
    //arguments are true if motors going forward, while members are false when motors are going forward...
    //so, if the same then resetTicks, and then set members to opposite of arguements.
    if (lefDir == reverseDirLeft || rigDir == reverseDirRight){
        resetTicks();
    }
    reverseDirLeft = !lefDir;
    reverseDirRight = !rigDir;
}

void Encoders::ticks(int &lef, int &rig){
    int l = left->Counts(), r = right->Counts();
    if (doubleSignal){
        l += leftDouble->Counts();
        r += rightDouble->Counts();
    }
    if (reverseDirLeft){
        lef = -l;
    } else {
        lef = l;
    }
    if (reverseDirRight){
        rig = -r;
    } else {
        rig = r;
    }
}

int Encoders::getRightDifferLeft(){
    int res = 0;
    //grab total counts for left and right as longs
    long aLef, aRig;
    archives(aLef,aRig);
    //find difference between the two longs and cast it to int
    //  Cast to int is safe for operating grounds of this, as even if turning one degree requires 100 counts in difference...
    //  Then can still do 174,221 full 90 degree turns in one direction before reaching 4/5 of the way to the int datatype limit.
    //  Or 43,555 complete 360 turns in one direction
    //  In comparison to Igwan Encoders for Expoloration 3, 90 degrees took roughly 400 counts in difference.
    res = (int)(aLef - aRig);
    return res;
}

void Encoders::enableDoubleSignal(){
    doubleSignal = true;
}
