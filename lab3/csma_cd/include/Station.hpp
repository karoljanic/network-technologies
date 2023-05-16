#ifndef STATION_HPP
#define STATION_HPP

#include <math.h>
#include <random>

#include "Cable.hpp"

class Cable;

enum class State {
    RECEIVING,
    TRANSMITTING,
    WAIT_TO_START_TRANSMITTING,
    COLLISION_WAIT
};

class Station {
    std::string name;
    State state;
    unsigned int position;
    Cable* cable;
    std::string signalColor;
    unsigned int sendingTime;
    int waitingTime;
    unsigned int sendingFails;
    std::string logger;

    std::mt19937 generator;

public:
    Station(std::string stationName, unsigned int positionOnCable, Cable* connectedCable, std::string color);

    std::string getName() const;
    unsigned int getPosition() const;
    std::string getSignalColor() const;
    std::string getState() const;
    std::string getLogs() const;

    void sendFrame();
    void updateState();

private:
    void startTransmitting();
    void solveCollision();
    void stopTransmitting();
}; 

#endif // STATION_HPP