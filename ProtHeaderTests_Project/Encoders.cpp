#include <Encoders.h>
#include <FEHProteus.h>
#include <fehproteusfirmware\Libraries\FEHIO.h>

Encoders::Encoders(){
    revLeft = false;
    revRight = false;
    archive_left = 0l;
    archive_right = 0l;
    left = nullptr;
    right = nullptr;
}

Encoders::Encoders(AnalogEncoder lef, AnalogEncoder rig){
    left = &lef;
    right = &rig;
    revLeft = false;
    revRight = false;
    archive_left = 0l;
    archive_right = 0l;
}

Encoders::~Encoders(){
    left = nullptr;
    right = nullptr;
}

void Encoders::archives(long &lef, long &rig){
    lef = archive_left + (*left).Counts();
    rig = archive_right + (*right).Counts();
}

void Encoders::setThresholds(float low, float high){
    left->SetThresholds(low, high);
    right->SetThresholds(low, high);
}

void Encoders::resetTicks(){
    int l = left->Counts(), r=right->Counts();
    if (revLeft){
        archive_left -= l;
    } else {
        archive_left += l;
    }
    if (revRight){
        archive_right -= r;
    } else {
        archive_right += r;
    }
    left->ResetCounts();
    right->ResetCounts();
}

void Encoders::setEncoders(AnalogEncoder lef, AnalogEncoder rig){
    left = &lef;
    right = &rig;
}

void Encoders::setRev(bool lefRev, bool rigRev){
    if (lefRev != revLeft || rigRev != revRight){
        resetTicks();
    }
    revLeft = lefRev;
    revRight = rigRev;
}

void Encoders::ticks(int &lef, int &rig){
    int l = left->Counts(), r = right->Counts();
    if (revLeft){
        lef = -l;
    } else {
        lef = l;
    }
    if (revRight){
        rig = -r;
    } else {
        rig = r;
    }
}
