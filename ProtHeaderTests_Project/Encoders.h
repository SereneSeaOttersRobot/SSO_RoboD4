#ifndef Encoders_H
#define Encoders_H
#include <fehproteusfirmware\Libraries\FEHIO.h>

class Encoders{
    private:
    long archive_left;
    long archive_right;
    bool revLeft;
    bool revRight;
    public:
    AnalogEncoder * left;
    AnalogEncoder * right;
    Encoders();
    Encoders(AnalogEncoder lef, AnalogEncoder rig);
    ~Encoders();
    void setEncoders(AnalogEncoder lef, AnalogEncoder rig);
    void ticks(int &lef, int &rig);
    void resetTicks();
    void setRev(bool lefRev, bool rigRev);
    void archives(long &lef, long &rig);
    void setThresholds(float low, float high);
};

#endif