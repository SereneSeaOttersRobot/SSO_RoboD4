#ifndef LightSensor_H
#define LightSensor_H
#include <fehproteusfirmware\Libraries\FEHIO.h>

#define Base_Blue_Light 0.94
#define Base_Red_Light 0.295
#define Base_Tolerance 0.05
#define Base_Pin FEHIO::FEHIOPin::P0_0

class LightSensor{
    private:
    float red_light;
    float blue_light;
    float tolerance;
    public:
    AnalogInputPin * cds_sensor;
    /**
     * @brief default constructor for this, uses #defgroup Base_* for all possible
     * @post successful construction of this
     * @bug Client must create cds object themselves then get this pointer to point to client's cds sensor, otherwise won't work.
     */
    LightSensor();
    /**
     * @brief most complex constructor of this, takes pin, cds red light, cds blue light, and tolerance values.
     * @param pin a memberof FEHIO::FEHIOPin enumeration, the hardware pin to read the cds sensor from.
     * @param red_light float value between 0 and 3.3 representing a red_light
     * @param blue_light float value between 0 and 3.3 representing a blue_light
     * @param tolerance float value to tolerance the light values, allowing for error acceptance.
     * @post successful construction of this
     * @bug Client must create cds object themselves then get this pointer to point to client's cds sensor, otherwise won't work.
     */
    LightSensor(FEHIO::FEHIOPin pin, float red_light, float blue_light, float tolerance);
    /**
     * @brief Uses the given pin but the rest of the values are set to base value from #defgroup Base
     * @param pin a memberof FEHIO::FEHIOPin enumeration, the hardware pin to read the cds sensor from.
     * @post successful construction of this
     * @bug Client must create cds object themselves then get this pointer to point to client's cds sensor, otherwise won't work.
     */
    LightSensor(FEHIO::FEHIOPin pin);
    /**
     * @brief sets the red light and blue values to the given parameters/arguements
     * @pre Requires this has been successfully been constructed
     * @param red_light float value between 0 and 3.3 representing a red_light
     * @param blue_light float value between 0 and 3.3 representing a blue_light
     * @post Ensures that color lights are changed
     */
    void setLights(float red_light, float blue_light);
    /**
     * @brief returns the light value detected by the cds sensor
     * @note returns cds.Value() of AnalogInputPin
     * @pre Requires this has been successfully constructed
     * @return light value detected as a float value between 0 and 3.3
     */
    float lightValue();
    /**
     * @brief checks for Red Light for #def Light Counts iterations
     * @note loops while current light value isn't in tolerant Red light, ending if is in tolerant Red or counter exceeds limits
     * @pre Requires this to have been successfully constructed
     * @returns true iff tolerant Red light detected, or false if counter exceeded.
     */
    bool detectRed();
    /**
     * @brief checks for Blue Light for #def Light Counts iterations
     * @note loops while current light value isn't in tolerant Blue light, ending if is in tolerant Blue or counter exceeds limits
     * @pre Requires this to have been successfully constructed
     * @returns true iff tolerant Blue light detected, or false if counter exceeded.
     */
    bool detectBlue();
    /**
     * @brief sets the tolerance of this to the given parameter/arguement
     * @param tolerance float value for tolerancing the colored light values
     *                  | suggested between 0.1 and 0.01
     * @pre Requires this to have been successfully constructed
     * @post Ensures the tolerance of this is set to the sent tolerance
     */
    void setTolerance(float tolerance);
    /**
     * @brief Colors for this class's colors red and blue, with empty for detected not of those two colors.
     * @enum LRED - Light color Red
     * @enum LBLUE - Light color Blue
     * @enum LEMPTY - Unknown Light color
     */
    typedef enum {
        LRED = 0,
        LBLUE, 
        LEMPTY
        //expandable
    }LColor;
    /**
     * @brief returns the detected color of light as a custom enumeration of this class.
     * @enum LColor : LRED,LBLUE,LEMPTY
     * @see LColor memberof LightSensor
     * @pre Requires this to have been successfully constructed
     * @return LColor associated with the detected Light Color or absence of known.
     */
    LightSensor::LColor detectColor();
};

#endif