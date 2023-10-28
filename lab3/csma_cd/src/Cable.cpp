#include "Cable.hpp"

Cable::Cable(unsigned int cableLength): 
    length{cableLength}, 
    medium{cableLength} { }


unsigned int Cable::getLength() const {
    return length;
}


bool Cable::isPositionEmpty(unsigned int position) const {
    return medium[position].empty();
} 


bool Cable::isCollisionOnPosition(unsigned int position) const {
    return medium[position].size() > 1;
}


void Cable::connectStation(Station* station) {
    stations[station->getPosition()] = station;
}


void Cable::visualise() {
    std::cout << "\033c";  // reset terminal

    std::cout << "{T}";
    for(unsigned int i = 0; i < length; i++) {
        std::map<unsigned int, Station*>::iterator station = stations.find(i);

        if(station != stations.end())
            std::cout << "\033[38;5;" << station->second->getSignalColor() << "m|[\033[0m";
        else
            std::cout << "[";
        
        int counter = 0;
        for(const Signal& signal: medium[i]) {
            counter++;
            if(signal.getType() == Type::DATA)
                std::cout << "\033[38;5;" << signal.getColor() << "m#\033[0m";
            else if(signal.getType() == Type::JAM)
                std::cout << "\033[38;5;" << signal.getColor() << "m!\033[0m";
        }
        for(;counter < stations.size(); counter++)
            std::cout << " ";
        
        if(station != stations.end())
            std::cout << "\033[38;5;" << station->second->getSignalColor() << "m]|\033[0m";
        else
            std::cout << "]";
    }
    std::cout << "{T}\n\n";

    std::cout << "STATES:\n";
    for(std::map<unsigned int, Station*>::iterator station = stations.begin(); station != stations.end(); ++station) {
        std::cout << station->second->getName() << ": " << station->second->getState() << std::endl;
    }

    std::cout << "\nLOGS:\n";
    for(std::map<unsigned int, Station*>::iterator station = stations.begin(); station != stations.end(); ++station) {
        std::cout << station->second->getName() << ":\n";
        std::cout << station->second->getLogs() << "\n";
    }
}


void Cable::initializeSignal(unsigned int position, std::string color) {
    medium[position].emplace_back(color, Direction::BOTH, Type::DATA, true, 2 * length);
}

void Cable::initializeJam(unsigned int position, std::string color) {
    medium[position].emplace_back(color, Direction::BOTH, Type::JAM, true, 2 * length);
}


void Cable::propagateSignal() {
    std::vector<std::vector<Signal>> newMedium(length);

    for(unsigned int i = 0; i < length; i++) {
        for(auto jt = medium[i].begin(); jt != medium[i].end(); ++jt) {
            if(jt->getDirection() == Direction::LEFT && i > 0) {
                newMedium[i - 1].emplace_back(jt->getColor(), Direction::LEFT, jt->getType(), false, 0);                   
            }
            
            if(jt->getDirection() == Direction::RIGHT && i < length - 1) {
                newMedium[i + 1].emplace_back(jt->getColor(), Direction::RIGHT, jt->getType(), false, 0);
            }

            if(jt->getDirection() == Direction::BOTH) {
                if(i > 0) {
                    newMedium[i - 1].emplace_back(jt->getColor(), Direction::LEFT, jt->getType(), false, 0);
                }
                
                if(i < length - 1) {
                    newMedium[i + 1].emplace_back(jt->getColor(), Direction::RIGHT, jt->getType(), false, 0);
                }
            }

            if(jt->isLastSignal() && jt->getTimeToLive() > 1)
                newMedium[i].emplace_back(jt->getColor(), jt->getDirection(), jt->getType(), true, jt->getTimeToLive() - 1); 
        }
    }

    medium = newMedium;
}

void Cable::endTransmitting(std::string stationName) {
    for(auto it = stations.begin(); it != stations.end(); ++it) {
        if(it->second->getName() == stationName) {
            for(Signal& signal: medium[it->first]) {
                if(signal.getColor() == it->second->getSignalColor())
                    signal.clearTimeToLive();
            }
        }
    }
}
