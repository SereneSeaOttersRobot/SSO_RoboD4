#ifndef LINEFOLLOWER_H
#define LINEFOLLOW_H

#include <FEHIO.h>
#include <Constants.h>
#include <FEHLCD.h>
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
     * @return true if white line detected
    */
    bool onWhite();
    /**
     * Reports wether this is on yellow line or not.
     * Side sensitive.
     * @return true if yellow line detected
    */
    bool onYellow();

    /**
     * Reports wether on color line or not.
     * @param color the colored line to try an detect. Yellow or White
     * @return true if color line is detected.
    */
    bool onColor(Color color);

    private:
    Side thisSide;
};

#endif