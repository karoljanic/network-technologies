#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <boost/crc.hpp>
#include <bitset>


constexpr int FRAME_LENGTH = 32;
constexpr int ESCAPE_COUNT = 6;
constexpr const char* ESCAPE_SEQ = "01111110";
constexpr int CRC_LENGTH = 32;


void generate(const std::string fileName, int bitsNumber) {
    std::fstream out(fileName, std::ios::out);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    std::string bits;
    for (int i = 0; i < bitsNumber; ++i) {
        out << std::to_string(dis(gen));
    }

    out.close();
}

std::string generateCRC32(const std::string& input) {
    boost::crc_32_type crc32;
    crc32.process_bytes(input.data(), input.size());

    std::bitset<32> crcBits(crc32.checksum());

    return crcBits.to_string();
}

void encodeFile(const std::string fileName) {
    std::fstream in(fileName, std::ios::in);
    std::fstream out("encoded_" + fileName, std::ios::out);

    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string fileContent = buffer.str();
    size_t fileSize = fileContent.size();
    int counter = 1;
    for(size_t pos = 0; pos < fileSize; pos += FRAME_LENGTH) {
        std::string frame = fileContent.substr(pos, FRAME_LENGTH);

        frame += generateCRC32(frame);
        int ones = 0;

        std::string singleOutputFrame;

        singleOutputFrame += ESCAPE_SEQ;
        for(char bit: frame) {
            singleOutputFrame += bit;
            if(bit == '1') {
                ones++;
                if(ones == (ESCAPE_COUNT - 1)) {
                    singleOutputFrame += '0';
                    ones = 0;
                }
            }
            else { // bit == '0'
                ones = 0;
            }
        }
        singleOutputFrame += ESCAPE_SEQ;

        std::cout << counter << ": " << singleOutputFrame << '\n';
        out << singleOutputFrame;
        counter++;
    }

    in.close();
    out.close();
}

void decodeFile(const std::string fileName) {
    std::fstream in(fileName, std::ios::in);
    std::fstream out("decoded_" + fileName, std::ios::out);

    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string fileContent = buffer.str();
    
    std::string frame = "";
    int ones = 0;
    
    for(char bit: fileContent) {
        if(bit == '1') {
            ones++;
            if(ones >= ESCAPE_COUNT + 1) { 
                // error: to many ones, no valid bit stream
                frame = "";
            }
            else {
                frame += bit;
            }
        }
        else { // bit == '0'
            if(ones == ESCAPE_COUNT) { // escape sequence found
                frame = frame.substr(0, frame.size() - ESCAPE_COUNT - 1);
                
                if(frame.size() > 32) {
                    std::string payload = frame.substr(0, frame.size() - CRC_LENGTH);
                    std::string crc = frame.substr(frame.size() - CRC_LENGTH, CRC_LENGTH);
                                        
                    if(generateCRC32(payload) == crc) {
                        std::cout << "CRC IS VALID. PAYLOAD = " << payload << '\n';
                    }
                    else {
                        std::cout << "CRC IS NOT VALID.\n";
                    }
                    
                    out << payload;
                }
                    
                frame = "";
            }
            else if(ones == (ESCAPE_COUNT - 1)) {
                // additional(added during encoding) '0' found
            }
            else {
                frame += bit;
            }

            ones = 0;
        }
    }

    in.close();
    out.close();
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cout << "Invalid number of arguments!\n";
        return -1;
    }

    std::string operation{argv[1]};
    std::string file{argv[2]};

    if(operation == "generate") {
        if(argc != 4) {
            std::cout << "Invalid number of arguments!\n";
            return -1;
        }

        int bitsNumber = atoi(argv[3]);
        generate(file, bitsNumber);
    }
    else if(operation == "encode") {
        encodeFile(file);
    }
    else if(operation == "decode") {
        decodeFile(file);
    }
    else {
        std::cout << "Invalid operation name!\n";
        return -1;
    }

    return 0;
}