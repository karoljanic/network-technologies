#include "Signal.hpp"

Signal::Signal(std::string signalColor, Direction direction, Type type, bool last, unsigned short liveTime): 
    direction(direction), 
    color(signalColor),
    signalType(type),
    timeToLive(liveTime),
    isLast{last} { }


Direction Signal::getDirection() const { 
    return direction; 
}


std::string Signal::getColor() const { 
    return color; 
}

Type Signal::getType() const {
    return signalType;
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

void Signal::clearTimeToLive() {
    timeToLive = 0;
}