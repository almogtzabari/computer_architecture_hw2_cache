#include <assert.h>
#include <cmath>
#include "Cache.h"
#include "Logger.h"

Cache::Cache(CacheConfig cfg) : config(cfg), sets(config.num_of_blocks / config.associativity,
                                                  Set(config.associativity, config.block_size)),
                                stats({0, 0, 0, 0, 0}) {
    LOG(TRACE) << "Cache constructed at " << this;
}


bool Cache::read(unsigned addr, bool update_hit_rate, bool update_lru, EVICTION_STATUS* eviction_status, unsigned *write_back_addr) {
    stats.total_cycles += config.cycles;
    Set& set = sets[getSetIDByAddress(addr)];
    unsigned tag = getTagByAddress(addr);
    bool hit = set.blockExists(tag);
    if(update_hit_rate){
        hit? stats.total_read_hit++ : stats.total_read_miss++;
    }
    if(hit && update_lru){
        set.updateLRU(tag);
    }

    return hit;
}

bool Cache::write(unsigned addr, bool update_hit_rate, bool update_cycles, EVICTION_STATUS* eviction_status, unsigned *write_back_addr) {
    if(update_cycles){
        stats.total_cycles += config.cycles;
    }
    Set& set = sets[getSetIDByAddress(addr)];
    unsigned tag = getTagByAddress(addr);
    bool hit = set.blockExists(tag);
    if(update_hit_rate){
        // We don't know if the block is supposed to be here
        hit? stats.total_write_hit++ : stats.total_write_miss++;
        if(hit){
            set.updateLRU(tag);
            set.setDirty(tag);
        }
    }
    else{
        // We know for sure that the block should or should not be here
        if(!hit){
            // The block was not suppose to be here - Insert new block
            *eviction_status = set.evict(write_back_addr);
            set.insertBlock(tag, addr);
            set.updateLRU(tag);
        }
        else{
            // The block was suppose to be here - Update the block
            set.updateLRU(tag);
            set.setDirty(tag);
        }
    }
    return hit;
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

Statistics Cache::getStats() {
    return this->stats;
}

bool Cache::blockExists(unsigned addr) {
    unsigned tag = getTagByAddress(addr);
    Set& set = sets[getSetIDByAddress(addr)];
    return set.blockExists(tag);
}

void Cache::invalidateBlock(unsigned addr) {
    assert(blockExists(addr));
    unsigned tag = getTagByAddress(addr);
    Set& set = sets[getSetIDByAddress(addr)];
    set.invalidate(tag);
}

void Cache::setBlockDirty(unsigned addr) {
    assert(blockExists(addr));
    unsigned tag = getTagByAddress(addr);
    Set& set = sets[getSetIDByAddress(addr)];
    set.setDirty(tag);
}

bool Cache::blockIsDirty(unsigned addr) {
    assert(blockExists(addr));
    unsigned tag = getTagByAddress(addr);
    Set& set = sets[getSetIDByAddress(addr)];
    return set.blockIsDirty(tag);
}

void Cache::setBlockNotDirty(unsigned addr) {
    assert(blockExists(addr));
    unsigned tag = getTagByAddress(addr);
    Set& set = sets[getSetIDByAddress(addr)];
    set.setNotDirty(tag);
}
