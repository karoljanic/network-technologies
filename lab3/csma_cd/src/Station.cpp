#include "Station.hpp"

Station::Station(std::string stationName, unsigned int positionOnCable, Cable* connectedCable, std::string color): 
    name(stationName), 
    state(State::RECEIVING),
    position(positionOnCable),
    cable(connectedCable),
    signalColor(color),
    sendingTime(0),
    waitingTime(0),
    sendingFails(0),
    generator(rand()) { }


std::string Station::getName() const { 
    return name; 
}


unsigned int Station::getPosition() const { 
    return position; 
}


std::string Station::getSignalColor() const { 
    return signalColor; 
}


std::string Station::getState() const {
    switch (state) {
    case State::RECEIVING:
        return "is receiving";
    case State::TRANSMITTING:
        return "is transmitting(" + std::to_string(sendingTime) + "/" + std::to_string(2 * cable->getLength()) + " bits)";
    case State::WAIT_TO_START_TRANSMITTING:
        return "is waiting to start transmitting";
    case State::COLLISION_WAIT:
        return "detected collision and is waiting(" + std::to_string(waitingTime) + " ticks)";
    }

    return "";
}

std::string Station::getLogs() const {
    return logger;
}


void Station::sendFrame() {
    if(state == State::RECEIVING) {
        sendingTime = 0;
        sendingFails = 0;
        state = State::WAIT_TO_START_TRANSMITTING;
    }
}


void Station::updateState() {
    if(state == State::COLLISION_WAIT) {
        if(waitingTime <= 0)
            state = State::WAIT_TO_START_TRANSMITTING;
        else
            waitingTime--;
    }

    if(state == State::WAIT_TO_START_TRANSMITTING && cable->isPositionEmpty(position)) {
        startTransmitting();
    }

    if(state == State::TRANSMITTING) {
        if(cable->isCollisionOnPosition(position))
            solveCollision();

        sendingTime++;
        if(sendingTime == (2 * cable->getLength() + 1)) {
            stopTransmitting();
            logger += "transmission completed!\n";
        }
    }
}


void Station::startTransmitting() {
    logger += "transmission started!\n";
    state = State::TRANSMITTING;
    sendingTime = 0;
    cable->initializeSignal(position, signalColor);
}


void Station::solveCollision() {
    logger += "detected collision!\n";
    stopTransmitting();
    cable->initializeJam(position, "255");
    sendingFails++;

    if(sendingFails > 16) {
        state = State::RECEIVING;
        logger += "transmission aborted!\n";
    }
    else {
        unsigned short fails;
        if(sendingFails > 10)
            fails = 10;
        else
            fails = sendingFails;

        state = State::COLLISION_WAIT;
        
        unsigned int maxWaitSlots = (2 << fails - 1) - 1;
        std::uniform_int_distribution<> dis(0, maxWaitSlots);
        int waitSlots = dis(generator);
        waitingTime = 2 * cable->getLength() * waitSlots;

        logger += ("waiting " + std::to_string(waitingTime) + "\n");
    }
}


void Station::stopTransmitting() {
    state = State::RECEIVING;
    cable->endTransmitting(name);
}
