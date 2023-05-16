#ifndef SIGNAL_HPP
#define SIGNEL_HPP

#include <string>

enum class Direction {
    LEFT,
    RIGHT,
    BOTH
};

class Signal {
    Direction direction;
    std::string color;
    unsigned short timeToLive;
    bool isLast;

public:
    Signal(std::string signalColor, Direction direction, bool back, unsigned short liveTime);

    Direction getDirection() const;
    std::string getColor() const;
    bool isLastSignal() const;
    unsigned short getTimeToLive() const;
    void decreaseTimeToLive();
    void clearTimeToLive();
};  

#endif // SIGNAL_HPP