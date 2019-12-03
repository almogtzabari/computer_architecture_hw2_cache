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
    Set& set = sets[getSetIDByAddress(addr)];
    unsigned tag = getTagByAddress(addr);
    bool hit = set.blockExists(tag);
    hit? stats.total_read_hit++ : stats.total_read_miss++;
    return hit;
}

void Cache::updateBlock(unsigned addr) {
    Set& set = sets[getSetIDByAddress(addr)];
    unsigned tag = getTagByAddress(addr);
    // TODO: Should we update ranking?
    assert(set.blockExists(tag));
}

unsigned Cache::getSetIDByAddress(unsigned addr) {
    assert(sets.size()); // At least one set should exist

    // Edge case - fully associative
    if (sets.size() == 1) {
        return 0;
    }

    unsigned mask = 0;
    for (auto i = 0; i < std::log2(sets.size()); i++) {
        mask += pow(2, i);
    }
    unsigned num_of_offset_bits = std::log2(config.block_size);
    return mask & (addr >> num_of_offset_bits);
}

unsigned Cache::getTagByAddress(unsigned addr) {
    unsigned num_of_set_bits = std::log2(sets.size());
    unsigned num_of_offset_bits = std::log2(config.block_size);
    return addr>>(num_of_offset_bits + num_of_set_bits);
}

unsigned Cache::evict(unsigned for_address) {
    unsigned set_id = getSetIDByAddress(for_address);
    Set& set = sets[set_id];
    return set.evict();
}

void Cache::insertBlock(unsigned addr) {
    unsigned tag = getTagByAddress(addr);
    unsigned set_id = getSetIDByAddress(addr);
    sets[set_id].insertBlock(tag, addr);
    // TODO: Should we add cycles?
}

bool Cache::write(unsigned addr) {
    Set& set = sets[getSetIDByAddress(addr)];
    unsigned tag = getTagByAddress(addr);
    bool hit = set.blockExists(tag);
    hit? stats.total_write_hit++ : stats.total_write_miss++;
    return hit;
}

void Cache::addCycles() {
    this->stats.total_cycles += this->config.cycles;
}
