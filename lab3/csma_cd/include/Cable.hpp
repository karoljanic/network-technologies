#ifndef CABLE_HPP
#define CABLE_HPP

#include <vector>
#include <map>
#include <ostream>
#include <iostream>

#include "Signal.hpp"
#include "Station.hpp"

class Station;

class Cable {
    unsigned int length;
    std::vector<std::vector<Signal>> medium;
    std::map<unsigned int, Station*> stations;

public:
    Cable(unsigned int cableLength);

    unsigned int getLength() const;

    bool isPositionEmpty(unsigned int position) const;
    bool isCollisionOnPosition(unsigned int position) const;

    void connectStation(Station* station);
    void visualise();
    void initializeSignal(unsigned int position, std::string color);
    void initializeJam(unsigned int position, std::string color);
    void propagateSignal();
    void endTransmitting(std::string stationName);
};

#endif // CABLE_HPP