#ifndef SIGNAL_HPP
#define SIGNEL_HPP

#include <string>

enum class Direction {
    LEFT,
    RIGHT,
    BOTH
};

enum class Type {
    DATA,
    JAM
};

class Signal {
    Direction direction;
    std::string color;
    Type signalType;
    unsigned short timeToLive;
    bool isLast;

public:
    Signal(std::string signalColor, Direction direction, Type type, bool back, unsigned short liveTime);

    Direction getDirection() const;
    std::string getColor() const;
    Type getType() const;
    bool isLastSignal() const;
    unsigned short getTimeToLive() const;
    void decreaseTimeToLive();
    void clearTimeToLive();
};  

#endif // SIGNAL_HPP