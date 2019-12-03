

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

    unsigned getSetIDByAddress(unsigned addr);
    unsigned getTagByAddress(unsigned addr);

public:
    Cache(CacheConfig cfg);
    bool read(unsigned addr);
    bool write(unsigned addr);
    void updateBlock(unsigned addr);
    void insertBlock(unsigned addr);
    void addCycles();

    /**
     * Evicts a block from cache and returns block address if needed to be written to lower level cache.
     * @param for_address - address of block to evict from its set.
     * @return block address need to be written, 0 otherwise.
     */
    unsigned evict(unsigned for_address);

};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H
