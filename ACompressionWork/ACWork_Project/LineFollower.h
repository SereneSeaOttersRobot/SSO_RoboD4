#ifndef LINEFOLLOWER_H
#define LINEFOLLOW_H

#include <FEHIO.h>
#include <Constants.h>
/**
 * Linefollower extends AnalogInputPin and adds additional functionality for use as a Line Sensor.
 * Added functions are sensitive to the Side the object is designated as.
*/
class LineFollower: public AnalogInputPin{

    public:
    /**
     * Create this as an AnalogInputPin with a argued side.
     * @param pin IO pin for analog input
     * @param side The side designation of this.
    */
    LineFollower(FEHIO::FEHIOPin pin, Side side);
    /**
     * Reports wether this is on white line or not.
     * Side sensitive.
    */
    bool onWhite();
    /**
     * Reports wether this is on yellow line or not.
     * Side sensitive.
    */
    bool onYellow();

    private:
    Side thisSide;
};

#endif