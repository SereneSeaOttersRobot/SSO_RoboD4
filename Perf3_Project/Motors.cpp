#include <Motors.h>
#include <FEHProteus.h>
#include <fehproteusfirmware\Libraries\FEHMotor.h>



Motors::Motors(){
    left = nullptr;
    right = nullptr;
    leftPositiveIsForward = true;
    rightPositiveIsForward = true;
}

Motors::Motors(FEHMotor &lef, FEHMotor &righ){
    left = &lef;
    right = &righ;
    leftPositiveIsForward = true;
    rightPositiveIsForward = true;
}

void Motors::setPerc(float perc){
    //follow pointers to the object at the held addresses, then use FEHMotor function to set percent of motors.
    if (leftPositiveIsForward){
        left->SetPercent(perc);
    } else {
        left->SetPercent(-perc);
    }
    if (rightPositiveIsForward){
        right->SetPercent(perc);
    } else {
        right->SetPercent(-perc);
    }
}

void Motors::setPerc(float lperc, float rperc){
    if (leftPositiveIsForward){
        left->SetPercent(lperc);
    } else {
        left->SetPercent(-lperc);
    }
    if (rightPositiveIsForward){
        right->SetPercent(rperc);
    } else {
        right->SetPercent(-rperc);
    }
}

void Motors::stop(){
    left->Stop();
    right->Stop();
}

void Motors::setMotorDirection(bool leftPosIsForward, bool rightPosIsForward){
    leftPositiveIsForward = leftPosIsForward;
    rightPositiveIsForward = rightPosIsForward;
}