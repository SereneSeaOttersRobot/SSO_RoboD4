#ifndef Encoders_H
#define Encoders_H
#include <fehproteusfirmware\Libraries\FEHIO.h>

class Encoders{
    private:
    long archive_left;
    long archive_right;
    bool reverseDirLeft; //true if direction is negative, i.e. opposite of setDir left bool.
    bool reverseDirRight; //true if direction is negative, i.e. opposite of setDir right bool.
    bool doubleSignal; //defualt is false
    public:
    /**
     * @brief pointer to left encoder.
     */
    AnalogEncoder * left;
    /**
     * @brief pointer to left double signal encoder
     */
    AnalogEncoder * leftDouble;
    /**
     * @brief pointer to right encoder.
     */
    AnalogEncoder * right;
    /**
     * @brief pointer to right double signal encoder
     */
    AnalogEncoder * rightDouble;
    /**
     * @brief default constructor
     */
    Encoders();
    /**
     * @brief constructs this with this.left pointing to lef and this.right pointing to rig.
     * @param lef In scope of calling program, AnalogEncoder Object to be pointed at for left encoder.
     * @param rig In scope of calling program, AnalogEncoder Object to be pointed at for right encoder.
     * @deprecated doesn't work for assigning pointers
     */
    Encoders(AnalogEncoder &lef, AnalogEncoder &rig);
    /**
     * @brief default destructor
     */
    ~Encoders();
    /**
     * @brief Similar to the constructor for this, but doesn't change the internal values besides the AnalogEncoders.
     * @deprecated doesn't work for assigning pointers
     */
    void setEncoders(AnalogEncoder &lef, AnalogEncoder &rig);
    /**
     * @brief Gets the current counts for left and right.
     * @param lef replaced with left encoder counts. Affected by setDir.
     * @param rig replaced with right encoder counts. Affected by setDir.
     */
    void ticks(int &lef, int &rig);
    /**
     * @brief resets the counters on the AnalogEncoder objects to zero. setDir can call this.
     */
    void resetTicks();
    /**
     * @brief Client sets the direction for what the encoders are reading, changing these values calls resetTicks, false values turn count values to negative in Ticks and for internal variables.
     * @param lefDir send true if left motor is going forward and false otherwise. Defaults is true.
     * @param rigDir send true if right motor is going forward and false otherwise. Default is true.
     */
    void setDir(bool lefDir, bool rigDir);
    /**
     * @brief returns the archive values (or total displacement values) for the left and right encoders from beginning of this object.
     * @param lef replaced with long value for archive left encoder counts + current left encoder counts. Affected by setDir.
     * @param rig replaced with long value for archive right encoder counts + current right encoder counts. Affected by setDir.
     */
    void archives(long &lef, long &rig);
    /**
     * @brief Sets the thresholds for both AnalogEncoders of this to the sent arguement values.
     * @param low the low threshold to set for the encoders.
     * @param high the high threshold to set for the encoders.
     */
    void setThresholds(float low, float high);
    /**
     * @brief Finds the difference between Right from Left. If left counts is greater than right, this returns postive, positve otherwise.
     * @return (this.archiveLeft + this.left.counts) - (this.archiveRight + this.right.counts) | Affected by setDir, uses archive values i.e. this.archives values
     */
    int getRightDifferLeft();

    /**
     * @brief enables double signal usage internally, need to assign double pointers as well.
     * 
     * Should be called before doing any other sets or getting motors moving. That and pointers should assigned before the rest.
     */
    void enableDoubleSignal();
};

#endif