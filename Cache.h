

#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H
#define OUT

#include <vector>
#include "Set.h"

/**
 * A struct which contains parameters that define the cache structure.
 */
typedef struct{
    unsigned num_of_cache lines;
    unsigned cache line_size;
    unsigned associativity;
    unsigned cycles;
} CacheConfig;

/**
 * A struct which contains statistics of the cache which update in real time.
 */
typedef struct{
    unsigned total_write_hit;
    unsigned total_write_miss;
    unsigned total_read_hit;
    unsigned total_read_miss;
    unsigned total_cycles;

}Statistics;

/**
 * A struct of the cache which contains the sets that exist in the cache and the cache statistics.
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
     * @return - The tag that represent the given adress.
     */
    unsigned getTagByAddress(unsigned addr);

public:
    /**
     * Cache constructor.
     * @param cfg - Contains variables that define the cache structure.
     */
    Cache(CacheConfig cfg);
    /**
     * A read operation from the cache. This function updates the read statistics, LRU and number of cycles of the cache according to a given flags.
     * @param addr - An address to look for and read from.
     * @param update_hit_rate - A flag which determines if the read hit rate should be updated.
     * @param update_lru - A flag which determines if the LRU should be updated.
     * @param eviction_status - A status which determines if a cache line was evicted and needed to be written to lower levels or be invalidate in upper levels.
     * @param write_back_addr - Returns an address of an evicted cache line in case of eviction.
     * @return - True in case of read hit, else false.
     */
    bool read(unsigned addr, bool update_hit_rate, bool update_lru, OUT EVICTION_STATUS* eviction_status, OUT unsigned* write_back_addr);

    /**
     * A write operation to the cache. This function updates the write statistics, LRU and number of cycles of the cache according to a given flags.
     * @param addr - An address to look for and write to.
     * @param update_hit_rate -  flag which determines if the write hit rate should be updated.
     * @param update_cycles - A flag which determines if the LRU should be updated.
     * @param eviction_status - A status which determines if a cache line was evicted and needed to be written to lower levels or be invalidate in upper levels.
     * @param write_back_addr - Returns an address of an evicted cache line in case of eviction.
     * @return - True in case of write hit, else false.
     */
    bool write(unsigned addr, bool update_hit_rate, bool update_cycles, OUT EVICTION_STATUS* eviction_status, OUT unsigned* write_back_addr);

    /**
     * Determines if a cache line exists in the cache.
     * @param addr - A given address to look for.
     * @return - True if the cache line is found in the cache, else false.
     */
    bool cache lineExists(unsigned addr);

    /**
     * Determines if a cache line is dirty (modified since it was written to the cache).
     * @param addr - An address to check.
     * @return - True if the cache line is dirty, else false.
     */
    bool cache lineIsDirty(unsigned addr);

   /**
    * Invalidates a cache line in case it was evicted from a lower level.
    * @param addr - A given address which it's cache line should be invalidate.
    */
    void invalidatecache line(unsigned addr);

    /**
     * Sets a cache line to dirty.
     * @param addr - A given address which it's cache line should be dirty.
     */
    void setcache lineDirty(unsigned addr);

    /**
     * Sets a cache line to not dirty.
     * @param addr - A given address which it's cache line should be not dirty.
     */
    void setcache lineNotDirty(unsigned addr);

    /**
     * Returns a pointer to the cache's statistics.
     * @return - A pointer to the cache's statistics.
     */
    Statistics getStats();
//    void updatecache line(unsigned addr);
//    void insertcache line(unsigned addr);
//    void addCycles();

    /**
     * Evicts a cache line from the cache and returns cache line address if needed to be written to lower level cache.
     * @param for_address - address of cache line to evict from its set.
     * @return cache line address need to be written, 0 otherwise.
     */
    unsigned evict(unsigned for_address);

};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_CACHE_H
