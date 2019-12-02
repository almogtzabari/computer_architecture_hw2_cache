#include <assert.h>
#include <cmath>
#include "Cache.h"
#include "Logger.h"

Cache::Cache(CacheConfig cfg) : config(cfg), sets(config.num_of_blocks / config.associativity,
                                                  Set(config.associativity, config.block_size)),
                                stats({0, 0, 0, 0, 0}) {
    LOG(TRACE) << "Cache constructed at " << this;
}

bool Cache::read(unsigned addr) {
    Set& set = getSetByAddress(addr);
    unsigned tag = getTagByAddress(addr);
    bool hit = set.blockExists(tag);
    stats.total_cycles += config.cycles;
    hit? stats.total_read_hit++ : stats.total_read_miss++;
    return hit;
}

bool Cache::write(unsigned addr) {
    Set& set = getSetByAddress(addr);
    unsigned tag = getTagByAddress(addr);
    bool hit = set.blockExists(tag);
    stats.total_cycles += config.cycles;
    hit? stats.total_write_hit++ : stats.total_write_miss++;
    return hit;
}

Set& Cache::getSetByAddress(unsigned addr) {
    assert(sets.size()); // At least one set should exist

    // Edge case
    if (sets.size() == 1) {
        return sets[0];
    }

    unsigned mask = 0;
    for (auto i = 0; i < sets.size(); i++) {
        mask += pow(2, i);
    }
    unsigned num_of_offset_bits = std::log2(config.block_size);
    return sets[mask & addr >> num_of_offset_bits];
}

unsigned Cache::getTagByAddress(unsigned addr) {
    unsigned num_of_set_bits = std::log2(sets.size());
    unsigned num_of_offset_bits = std::log2(config.block_size);
    return addr>>(num_of_offset_bits + num_of_set_bits);
}