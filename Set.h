
#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_SET_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_SET_H
#define OUT

#include <vector>
#include "CacheLine.h"

typedef enum{
    WRITE_BACK = 0,
    INVALIDATE = 1,
    NOTHING = 2
} EVICTION_STATUS;

class Set {
private:
    std::vector<CacheLine> lines;
    std::vector<unsigned> ranking;

public:
    Set(unsigned associativity, unsigned block_size);
    bool blockExists(unsigned tag);
    bool write(unsigned addr, unsigned tag);
    EVICTION_STATUS evict(OUT unsigned* write_back_addr);
    void insertBlock(unsigned tag, unsigned addr);
    void setDirty(unsigned tag);
    void invalidate(unsigned tag);
    void updateLRU(unsigned tag);
};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_SET_H
