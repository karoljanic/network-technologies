#include "Signal.hpp"

Signal::Signal(std::string signalColor, Direction direction, bool last, unsigned short liveTime): 
    direction(direction), 
    color(signalColor),
    timeToLive(liveTime),
    isLast{last} { }


Direction Signal::getDirection() const { 
    return direction; 
}


std::string Signal::getColor() const { 
    return color; 
}


bool Signal::isLastSignal() const {
    return isLast;
}


unsigned short Signal::getTimeToLive() const {
    return timeToLive;
}


void Signal::decreaseTimeToLive() { 
    timeToLive--;
}