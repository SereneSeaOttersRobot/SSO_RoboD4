#include <Motors.h>
#include <FEHProteus.h>
#include <fehproteusfirmware\Libraries\FEHMotor.h>

class Motors {
    public:
    /**
     * @brief pointer to the left motor object
     * @memberof FEHMotor
     */
    FEHMotor * left;
    /**
     * @brief pointer to the right motor object
     * @memberof FEHMotor
     */
    FEHMotor * right;
    /**
     * @brief default constructor, left and right point to nothing.
     * @param left pointer to left motor object
     * @param right pointer to right motor object
     */
    Motors();
    /**
     * @brief Takes ports for left and right motors and the voltage to set them at.
     * @param lef enumeration Port for left motor object
     * @param righ enumeration Port for right motor object
     * @param voltage the float voltage that motors will be set to
     * @post This is fully created object, accessing data members through pointers or can use functions after this
     */
    Motors(FEHMotor::FEHMotorPort lef, FEHMotor::FEHMotorPort righ, float voltage);
    /**
     * @overload this function is overloaded
     * @brief sets both member motors to the percent given
     * @pre requires this has been successfully constructed
     * @param perc float percent to set both motors to
     * @post both motors are set to perc
     */
    void setPerc(float perc);
    /**
     * @overload this function is overloaded
     * @brief sets left motor to the first percent and the right motor to the second percent.
     * @pre requires this has been successfully constructed
     * @param lperc float percent to set the left motor object to
     * @param rperc float percent to set the right motor object to
     * @post left motor follows lperc and right motor follows rperc
     */
    void setPerc(float lperc, float rperc);
    /**
     * @brief stops both motors, similar to @headerfile FEHMotor.h Stop().
     * @pre requires this has been successfully constructed
     * @post both motors stop moving
     */
    void stop();
};


Motors::Motors(){};

Motors::Motors(FEHMotor::FEHMotorPort lef, FEHMotor::FEHMotorPort rig, float voltage){
    FEHMotor eft(lef,voltage);
    FEHMotor ight(rig,voltage);
    left = &eft;
    right = &ight;
};

void Motors::setPerc(float perc){
    //follow pointers to the object at the held addresses, then use FEHMotor function to set percent of motors.
    left->SetPercent(perc);
    right->SetPercent(perc);
};

void Motors::setPerc(float lperc, float rperc){
    left->SetPercent(lperc);
    right->SetPercent(rperc);
};

void Motors::stop(){
    left->Stop();
    right->Stop();
};