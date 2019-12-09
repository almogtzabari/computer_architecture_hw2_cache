

#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H
#define OUT

#include <vector>
#include "Set.h"

/**
 * Parameters that define the cache structure.
 */
typedef struct{
    unsigned num_of_blocks;
    unsigned block_size;
    unsigned associativity;
    unsigned cycles;
} CacheConfig;

/**
 * Statistics of the cache which update in real time.
 */
typedef struct{
    unsigned total_write_hit;
    unsigned total_write_miss;
    unsigned total_read_hit;
    unsigned total_read_miss;
    unsigned total_cycles;

}Statistics;

/**
 * The cache itself, contains sets of blocks that exist in it and statistics.
 */
class Cache {
private:
    CacheConfig config;
    std::vector<Set> sets;
    Statistics stats;
    /**
     * Extracts the bits that represent the address's set.
     * @param addr - A given memory address.
     * @return - The correct set's index by a given address.
     */
    unsigned getSetIDByAddress(unsigned addr);

    /**
     * Extracts the bits that represent the address's tag.
     * @param addr - A given memory address.
     * @return - The tag that represent the given address.
     */
    unsigned getTagByAddress(unsigned addr);

public:
    /**
     * Cache constructor.
     * @param cfg - Contains variables that define the cache structure.
     */
    Cache(CacheConfig cfg);
    /**
     * A read operation from the cache. Updates the read statistics, LRU and number of cycles of the cache according to a given flags.
     * @param addr - An address to look for and read from.
     * @param update_hit_rate - A flag which determines if the read hit rate should be updated.
     * @param update_lru - A flag which determines if the LRU should be updated.
     * @param eviction_status - A status which determines if a block was evicted and needed to be written to lower levels or to be invalidated in upper levels.
     * @param write_back_addr - Returns an address of an evicted block in case of eviction.
     * @return - True in case of read hit, else false.
     */
    bool read(unsigned addr, bool update_hit_rate, bool update_lru, OUT EVICTION_STATUS* eviction_status, OUT unsigned* write_back_addr);

    /**
     * A write operation to the cache.Updates the write statistics, LRU and number of cycles of the cache according to a given flags.
     * @param addr - An address to look for and write to.
     * @param update_hit_rate -  flag which determines if the write hit rate should be updated.
     * @param update_cycles - A flag which determines if the LRU should be updated.
     * @param eviction_status - A status which determines if a block was evicted and needed to be written to lower levels or to be invalidated in upper levels.
     * @param write_back_addr - Returns an address of an evicted block in case of eviction.
     * @return - True in case of write hit, else false.
     */
    bool write(unsigned addr, bool update_hit_rate, bool update_cycles, OUT EVICTION_STATUS* eviction_status, OUT unsigned* write_back_addr);

    /**
     * Determines if a block exists in the cache.
     * @param addr - A given address to look for.
     * @return - True if the block is found in the cache, else false.
     */
    bool blockExists(unsigned addr);

    /**
     * Determines if a block is dirty (modified since it was written to the cache).
     * @param addr - An address to check.
     * @return - True if the block is dirty, else false.
     */
    bool blockIsDirty(unsigned addr);

   /**
    * Invalidates a block in case it was evicted from a lower level and updates LRU.
    * @param addr - A given address which it's block should be invalidate.
    */
    void invalidateBlock(unsigned addr);

    /**
     * Sets a block to dirty.
     * @param addr - A given address which it's block should be dirty.
     */
    void setBlockDirty(unsigned addr);

    /**
     * Sets a block to not dirty.
     * @param addr - A given address which it's block should be not dirty.
     */
    void setBlockNotDirty(unsigned addr);

    /**
     * Returns a pointer to the cache's statistics.
     * @return - A pointer to the cache's statistics.
     */
    Statistics getStats();
//    void updateBlock(unsigned addr);
//    void insertBlock(unsigned addr);
//    void addCycles();

    /**
     * Evicts a block from the cache and returns it's address if needed to be written to lower levels.
     * @param for_address - address of block to evict from its set.
     * @return block address need to be written, 0 otherwise.
     */
    unsigned evict(unsigned for_address);

};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H
