#include <ForkLift.h>
#include <Constants.h>
#include <FEHIO.h>
#include <FEHMotor.h>

ForkLift::ForkLift(FEHMotorPort motorPort, float voltage) : FEHMotor(motorPort, voltage) {}

void ForkLift::setButtonPins(FEHIO::FEHIOPin topPin, FEHIO::FEHIOPin bottomPin, FEHIO::FEHIOPin frontPin){
    this->topButton = DigitalInputPin(topPin);
    this->bottomButton = DigitalInputPin(bottomPin);
    this->frontButton = DigitalInputPin(frontPin);
}

void ForkLift::up(){
    if (this->topButton.Value() == BNP){
        SetPercent(fullPercentUp);
    }
}

void ForkLift::down(){
    if (this->bottomButton.Value() == BNP){
        SetPercent(-fullPercentUp);
    }
}

void ForkLift::toTop(){
    SetPercent(fullPercentUp);
    while (this->topButton.Value() == BNP);
    FEHMotor::Stop();
}

void ForkLift::toBottom(){
    SetPercent(-fullPercentUp);
    while (this->bottomButton.Value() == BNP);
    FEHMotor::Stop();
}

void ForkLift::Stop(){
    FEHMotor::Stop();
}

bool ForkLift::top(){
    return this->topButton.Value();
}

bool ForkLift::bottom(){
    return this->bottomButton.Value();
}

bool ForkLift::front(){
    return this->frontButton.Value();
}