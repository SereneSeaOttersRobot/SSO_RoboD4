#ifndef LINEFOLLOWER_H
#define LINEFOLLOW_H

#include <FEHIO.h>
#include <Constants.h>

class LineFollower: public AnalogInputPin{
    using AnalogInputPin::AnalogInputPin;

    public:
    /**
     * Create this as an AnalogInputPin with a argued side.
     * @param pin IO pin for analog input
     * @param side The side designation of this.
    */
    LineFollower(FEHIO::FEHIOPin pin, Side side);
    /**
     * Reports wether this is on white line or not.
    */
    bool onWhite();
    /**
     * Reports wether this is on white line or not.
    */
    bool onYellow();

    private:
    Side thisSide;
};

#endif