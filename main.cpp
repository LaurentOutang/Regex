#include <iostream>
#include <sstream>
#include "Regex.h"

int main()
{
    std::string digit0_9 = "0123456789|||||||||";
    std::string digit0_5 = "012345|||||";
    std::string digit0_4 = "01234||||";
    std::string digit00_99 = digit0_9 + digit0_9  + '.';
    std::string digit000_255 = "01|" + digit0_9 + '.' + digit0_9 + '.' + '2' + digit0_4 + '.' + digit0_9 + ".|" + "25." + digit0_5 + ".|";
    std::string eightBitNumber = digit0_9 + digit00_99 + '|' + digit000_255 + '|';
    std::string ipAdress = eightBitNumber + "\\.." + eightBitNumber + ".\\.." + eightBitNumber + ".\\.." + eightBitNumber + '.';
    std::string ipAdressSpaceSeparated = ipAdress + ' ' + ipAdress + ".*.";
    std::cout << ipAdressSpaceSeparated << std::endl;
    Regex reg(ipAdressSpaceSeparated);
    bool match = reg.match("056.0.127.251 23.52.012.1 000.0.00.12 1.2.3.4");
    std::cout << (match ? "ok" : "pas ok") << std::endl;
}