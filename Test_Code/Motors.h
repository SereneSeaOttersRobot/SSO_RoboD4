#ifndef Motors_H
#define Motors_H
#include <fehproteusfirmware\Libraries\FEHMotor.h>

class Motors {
    private:
    bool leftPositiveIsForward;
    bool rightPositiveIsForward;
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
     * @brief default constructor, left and right point to nothing. Defaults are set for privates.
     * @param left pointer to left motor object is set to null
     * @param right pointer to right motor object is set to null
     */
    Motors();
    /**
     * @brief Takes ports for left and right motors and the voltage to set them at. Defaults are set for privates.
     * @param lef address of in scope FEHMotor object for left motor
     * @param righ address of in scope FEHMotor object for right motor
     * @post This is fully created object, accessing data members through pointers or can use functions after this
     */
    Motors(FEHMotor &lef, FEHMotor &righ);
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
    /**
     * @brief updates internal logic to handle cases where setPercent values have opposite effects. So that after this, the setPercent function works correctly for single argument.
     * @pre requires this has been successfully constructed
     * @param leftPosIsForward send true if postive values make the motor go 'forward', false otherwise. Default is true.
     * @param rightPosIsForward send true if postive values make the motor go 'forward', false otherwise. Default is true.
     * @post internal logic is updated so that setPercent(15.0) makes both motors go forward.
     */
    void setMotorDirection(bool leftPosIsForward, bool rightPosIsForward);
};

#endif