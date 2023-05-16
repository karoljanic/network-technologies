#include <iostream>
#include <fstream>
#include <stdio.h>


#include "Cable.hpp"
#include "Station.hpp"

const unsigned int CABLE_LENGTH = 20;

const unsigned int STATION_1_POS = 0;
const char* STATION_1_NAME = "Red   ";

const unsigned int STATION_2_POS = 19;
const char* STATION_2_NAME = "Green ";

const unsigned int STATION_3_POS = 10;
const char* STATION_3_NAME = "Yellow";


int main() {
    Cable cable{CABLE_LENGTH};
    std::vector<Station> stations;

    stations.emplace_back(STATION_1_NAME, STATION_1_POS, &cable, "9");
    stations.emplace_back(STATION_2_NAME, STATION_2_POS, &cable, "10");
    stations.emplace_back(STATION_3_NAME, STATION_3_POS, &cable, "11");

    for(auto& station: stations) {
        cable.connectStation(&station);
    }

    cable.visualise();

    while(true) {
        int keyboardInput = getchar();

        if(keyboardInput == '1')
            stations[0].sendFrame();
        else if(keyboardInput == '2')
            stations[2].sendFrame();
        else if(keyboardInput == '3')
            stations[1].sendFrame();

        cable.propagateSignal();

        for(Station& station: stations) {
            station.updateState();
        }

        cable.visualise();
    }

    return 0;
}