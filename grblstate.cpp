#include "grblstate.h"

GrblState::GrblState():
    absolute(false),
    spindle(false), direction(false),
    coolant(false),
    toolNumber(-1),
    feedRate(-1)
{
}
