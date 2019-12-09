#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_MEMORYMANAGER_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_MEMORYMANAGER_H

#include <stdint-gcc.h>
#include "Logger.h"
#include "Cache.h"

/**
 * An enum that defines the writing policy.
 */
typedef enum{
    WRITE_NO_ALLOCATE = 0,
    WRITE_ALLOCATE = 1
}WritePolicy;

/**
 * Defines the structure of the different caches, access times to caches and memory, associativity and writing policy.
 */
typedef struct{
    unsigned memory_cycles;     // Number of cycles to access the main memory.
    unsigned block_size;        // Block size in bytes.
    WritePolicy policy;         // Write policy (write allocate or no updateBlock allocate).
    unsigned l1_size;           // L1 size in bytes.
    unsigned l1_associativity;  // L1 associativity.
    unsigned l1_cycle;          // Number of cycles to access L1.
    unsigned l2_size;           // L2 size in bytes.
    unsigned l2_associativity;  // L2 associativity.
    unsigned l2_cycle;          // Number of cycles to access L2.

} MMConfig;

/**
 * Statistics of miss rate of each cache and average access time.
 */
typedef struct{
    double miss_rate_l1;
    double miss_rate_l2;
    double average_access_time;
} SimStats;

/**
 * Statistics of total read and write commands and total memory cycles.
 */
typedef struct {
    unsigned memory_cycles;
    unsigned total_reads;
    unsigned total_writes;
} MMStats;

/**
 * The memory "itself", includes the different caches which are built according to the config, and saves different statistics.
 */
class MemoryManager {
private:
    MMConfig config;
    Cache L1;
    Cache L2;
    MMStats stats;

public:
    MemoryManager(MMConfig cfg);

    /**
     * A read operation from the memory.
     * @param addr - A memory address to look for.
     */
    void read(unsigned addr);

    /**
     * A write operation to the memory.
     * @param addr - A memory address to look for.
     */
    void write(unsigned addr);

    /**
     * Calculates hit rates of each cache and average access time.
     * @return
     */
    SimStats getStats();

};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_MEMORYMANAGER_H
