
#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_SET_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_SET_H


#include <vector>
#include "CacheLine.h"

class Set {
private:
    std::vector<CacheLine> lines;
    std::vector<unsigned> ranking;

public:
    Set(unsigned associativity, unsigned block_size);
    bool blockExists(unsigned tag);
    bool write(unsigned addr, unsigned tag);
};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_SET_H
