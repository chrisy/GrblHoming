#ifndef GRBLSTATE_H
#define GRBLSTATE_H

#include <QMetaType>

class GrblState
{
public:
    GrblState();

public:
    // G0/G1/G2/G3 active motion mode

    // G20/G21 inches/mm
    bool mm;
    bool in;

    // G17/G18/G19 active plane

    // G54-G59 work coordinate system

    // G90/91 abs/relative mode
    bool absolute;

    // G93/G94 inverse feed

    // M0/M1/M2 program flow

    // M3/M4/M5 spindle
    bool spindle;
    bool direction;

    // M7/M8/M9 coolant
    bool coolant;

    // T tool number
    int toolNumber;

    // F feed rate
    int feedRate;

    // %sx grbl switches
};

Q_DECLARE_METATYPE ( GrblState )

#endif // GRBLSTATE_H
