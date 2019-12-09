
#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_SET_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_SET_H
#define OUT

#include <vector>
#include "CacheLine.h"

/**
 * An eviction status. If a block was evicted and is dirty it need to be written to lower levels. If the block was evicted from L2,
 * it is also found in L1 and needs to be invalidated.
 */
typedef enum{
    WRITE_BACK = 0,
    INVALIDATE = 1,
    NOTHING = 2
} EVICTION_STATUS;

/**
 * A set the contains the different blocks and their ranking (the ranking is determined by last recently used policy).
 */
class Set {
private:
    std::vector<CacheLine> lines;
    std::vector<unsigned> ranking;

public:
    /**
     * Constructor.
     * @param associativity - cache's associativity.
     * @param block_size - cache's block size.
     */
    Set(unsigned associativity, unsigned block_size);

    /**
     * Determines if a block exists by it's tag.
     * @param tag - A given address's tag.
     * @return - True if the block exists and is valid, else false.
     */
    bool blockExists(unsigned tag);

    /**
     * Determines if a block is dirty.
     * @param tag - A given address's tag.
     * @return - True if the block is dirty and valid, else false.
     */
    bool blockIsDirty(unsigned tag);

    /**
     * Evicts a block, updates the ranking, and returns an address of the evicted block and an eviction status.
     * @param write_back_addr - The evicted block's address.
     * @return - A status that determines if the evicted block should be written to lower levels or be invalidated in upper levels.
     */
    EVICTION_STATUS evict(OUT unsigned* write_back_addr);

    /**
     * Inserts a block to a set.
     * @param tag - A given address's tag.
     * @param addr - A given memory address.
     */
    void insertBlock(unsigned tag, unsigned addr);

    /**
     * Sets a block as dirty.
     * @param tag - A given memory address's tag.
     */
    void setDirty(unsigned tag);

    /**
     * Sets a block as not dirty.
     * @param tag - A given memory address's tag.
     */
    void setNotDirty(unsigned tag);

    /**
     * Invalidates a block and updates LRU so that the invalidated block will be last in the ranking (was least recently used).
     * @param tag - A given memory address's tag.
     */
    void invalidate(unsigned tag);

    /**
     * Updates LRU so that the block with the given tag will be first in the ranking (was most recently used).
     * @param tag - A given memory address's tag.
     */
    void updateLRU(unsigned tag);
};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_SET_H
