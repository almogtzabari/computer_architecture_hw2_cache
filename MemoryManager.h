#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_MEMORYMANAGER_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_MEMORYMANAGER_H

#include <stdint-gcc.h>
#include "Logger.h"
#include "Cache.h"

typedef enum{
    WRITE_NO_ALLOCATE = 0,
    WRITE_ALLOCATE = 1
}WritePolicy;

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


class MemoryManager {
private:
    MMConfig config;
    Cache L1;
    Cache L2;



public:
    MemoryManager(MMConfig cfg);

    void read(unsigned addr);
    void write(unsigned addr);


};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_MEMORYMANAGER_H
