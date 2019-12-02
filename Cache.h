

#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H

#include <vector>
#include "Set.h"

typedef struct{
    unsigned num_of_blocks;
    unsigned block_size;
    unsigned associativity;
    unsigned cycles;
} CacheConfig;

typedef struct{
    unsigned total_write_hit;
    unsigned total_write_miss;
    unsigned total_read_hit;
    unsigned total_read_miss;
    unsigned total_cycles;

}Statistics;

class Cache {
private:
    CacheConfig config;
    std::vector<Set> sets;
    Statistics stats;

    Set& getSetByAddress(unsigned addr);
    unsigned getTagByAddress(unsigned addr);

public:
    Cache(CacheConfig cfg);
    bool read(unsigned addr);
    bool write(unsigned addr);

};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H
