#ifndef FOKRLIFT_H
#define FORKLIFT_H

#include <FEHIO.h>
#include <FEHMotor.h>
#include <Constants.h>

class ForkLift: private FEHMotor{

    public:
    
    /**
     * Creates the ForkLift object with the specificed Motor port as IO and Voltage
    */
    ForkLift(FEHMotorPort motorPort, float voltage);

    /**
     * sets up top, bottom, and front buttons of the Forklift object with the provided pins
    */
    void setButtonPins(FEHIO::FEHIOPin topPin, FEHIO::FEHIOPin bottomPin, FEHIO::FEHIOPin frontPin);
    
    /**
     * Moves forklift up at hundred percent motor speed.
     * Does not stop at top if left alone.
    */
    void up();

    /**
     * Moves forklift down at hundred percent motor speed.
     * Does not stop at bottom if left alone.
    */
    void down();

    /**
     * Stops forklift movement
    */
    void Stop();

    /**
     * Moves forklift to the very top and stops.
    */
    void toTop();

    /**
     * Moves forklift to the very bottom and stops.
    */
    void toBottom();

    /**
     * Returns BP if top button is pressed and BNP otherwise
    */
    bool top();

    /**
     * Returns BP if bottom button is pressed and BNP otherwise
    */
    bool bottom();

    /**
     * Returns BP if front button is pressed and BNP otherwise
    */
    bool front();

    private:
    DigitalInputPin topButton = DigitalInputPin(Null_Port);
    DigitalInputPin bottomButton = DigitalInputPin(Null_Port);
    DigitalInputPin frontButton = DigitalInputPin(Null_Port);
    float fullPercentUp = ForkLift_Sign_Up * 100.0;
};

#endif