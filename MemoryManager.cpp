#include <cmath>
#include "MemoryManager.h"
#include <assert.h>

MemoryManager::MemoryManager(MMConfig cfg) : config(cfg),
                                             L1({config.l1_size/config.block_size, config.block_size,
                                                 config.l1_associativity, config.l1_cycle}),
                                             L2({config.l2_size/config.block_size, config.block_size,
                                                 config.l2_associativity, config.l2_cycle}), stats({0 , 0, 0}) {
    LOG(TRACE) << "MemoryManager constructed at " << this;
}

void MemoryManager::read(unsigned addr) {
    LOG(DEBUG) << "MemoryManager reads address " << addr << ".";
    stats.total_reads++;
    addr = addr - (addr % config.block_size); // Calculating the address's block starting address.
    EVICTION_STATUS eviction_status = NOTHING;
    unsigned write_back_addr = 0;

    LOG(DEBUG) << "Looking for address " << addr << " in L1.";
    if(!L1.read(addr, true, true, &eviction_status, &write_back_addr)){
        /** Block is not in L1 */

        LOG(DEBUG) << "Address " << addr << " was not found in L1.";
        eviction_status = NOTHING;

        LOG(DEBUG) << "Looking for address " << addr << " in L2.";
        if(!L2.read(addr, true, true, &eviction_status, &write_back_addr)){
            /** Block is not in L1 and not in L2 */

            LOG(DEBUG) << "Address " << addr << " was not found in L2.";
            eviction_status = NOTHING;

            // Fetch block from main memory
            LOG(DEBUG) << "Fetching from main memory!";
            this->stats.memory_cycles += config.memory_cycles;

            // Insert the block fetched from main memory into L2
            LOG(DEBUG) << "Inserting address " << addr << " to L2.";
            L2.write(addr, false, false, &eviction_status, &write_back_addr);
            if(eviction_status == WRITE_BACK){
                // Evict block from L2 to main memory
                LOG(DEBUG) << "Address " << write_back_addr << " was evicted from L2 during insertion.";
                LOG(DEBUG) << "Writing address " << write_back_addr << " to the main memory.";
                if(L1.blockExists(write_back_addr)){
                    LOG(DEBUG) << "Address " << write_back_addr << " is also in L1. Invalidating it!";
                    L1.invalidateBlock(write_back_addr);
                }
            }
            else if(eviction_status == INVALIDATE){
                if(L1.blockExists(write_back_addr)){
                    LOG(DEBUG) << "Address" << write_back_addr << " was evicted from L2 during insertion but does not need to be written back.";
                    LOG(DEBUG) << "Address " << write_back_addr << " is also in L1. Invalidating it!";
                    L1.invalidateBlock(write_back_addr);
                }
            }

            eviction_status = NOTHING;

            // Insert the block also to L1
            LOG(DEBUG) << "Inserting address " << addr << " to L1.";
            L1.write(addr, false, false, &eviction_status, &write_back_addr);
            if(eviction_status == WRITE_BACK){
                // Evict block from L1 to L2
                LOG(DEBUG) << "Address " << write_back_addr << " was evicted from L1 during insertion.";
                LOG(DEBUG) << "Writing address " << write_back_addr << " to L2.";
                L2.write(write_back_addr, false, false, &eviction_status, &write_back_addr);
                eviction_status = NOTHING;
            }
        }
        else{
            /** Block is not in L1 but is in L2 */
            LOG(DEBUG) << "Address " << addr << " was found in L2.";


            // Insert the block fetched from L2 into L1
            LOG(DEBUG) << "Inserting address " << addr << " to L1.";
            L1.write(addr, false, false, &eviction_status, &write_back_addr);
            if(L2.blockIsDirty(addr)){
                L1.setBlockDirty(addr);
                L2.setBlockNotDirty(addr);
            }
            if(eviction_status == WRITE_BACK){
                // Evict block from L1 to L2
                LOG(DEBUG) << "Address" << write_back_addr << " was evicted from L1 during the insertion.";
                LOG(DEBUG) << "Writing address " << write_back_addr << " to L2.";
                L2.write(write_back_addr, false, false, &eviction_status, &write_back_addr);
            }
            eviction_status = NOTHING;
        }

    }
    else{
        /** Block is in L1 */
        // Noting to be done
        LOG(DEBUG) << "Address " << addr << " was found in L1";
    }
}


void MemoryManager::write(unsigned int addr) {
    LOG(DEBUG) << "MemoryManager writes address " << addr << ".";
    stats.total_writes++;
    addr = addr - (addr % config.block_size);
    EVICTION_STATUS eviction_status = NOTHING;
    unsigned write_back_addr = 0;

    LOG(DEBUG) << "Looking for address " << addr << " in L1.";
    if(!L1.write(addr, true, true, &eviction_status, &write_back_addr)){ // If hit - automatically set dirty
        /** Block is not in L1 */

        LOG(DEBUG) << "Address " << addr << " was not found in L1.";
        eviction_status = NOTHING;

        LOG(DEBUG) << "Looking for address " << addr << " in L2.";
        if(!L2.write(addr, true, true, &eviction_status, &write_back_addr)){ // If hit - automatically set dirty
            /** Block is not in L1 and not in L2 */

            LOG(DEBUG) << "Address " << addr << " was not found in L2.";
            eviction_status = NOTHING;

            // Fetch block from main memory
            LOG(DEBUG) << "Fetching from main memory!";
            this->stats.memory_cycles += config.memory_cycles;

            // Insert the block fetched from main memory into L2
            if(config.policy){
                LOG(DEBUG) << "Write allocate is on!";
                LOG(DEBUG) << "Inserting address " << addr << " to L2.";
                L2.write(addr, false, false, &eviction_status, &write_back_addr);
                if(eviction_status == WRITE_BACK){
                    // Evict block from L2 to main memory
                    LOG(DEBUG) << "Address " << write_back_addr << " was evicted from L2 during insertion.";
                    LOG(DEBUG) << "Writing address " << write_back_addr << " to the main memory.";
                    if(L1.blockExists(write_back_addr)){
                        LOG(DEBUG) << "Address " << write_back_addr << " is also in L1. Invalidating it!";
                        L1.invalidateBlock(write_back_addr);
                    }
                    eviction_status = NOTHING;
                }
                else if(eviction_status == INVALIDATE){
                    if(L1.blockExists(write_back_addr)){
                        LOG(DEBUG) << "Address" << write_back_addr << " was evicted from L2 during insertion but does not need to be written back.";
                        LOG(DEBUG) << "Address " << write_back_addr << " is also in L1. Invalidating it!";
                        L1.invalidateBlock(write_back_addr);
                    }
                }

                // Insert the block also to L1
                LOG(DEBUG) << "Inserting address " << addr << " to L1.";
                L1.write(addr, false, false, &eviction_status, &write_back_addr);
                L1.setBlockDirty(addr);
                if(eviction_status == WRITE_BACK){
                    // Evict block from L1 to L2
                    LOG(DEBUG) << "Address " << write_back_addr << " was evicted from L1 during insertion.";
                    LOG(DEBUG) << "Writing address " << write_back_addr << " to L2.";
                    L2.write(write_back_addr, false, false, &eviction_status, &write_back_addr);
                    eviction_status = NOTHING;
                }
            }
        }
        else{
            /** Block is not in L1 but is in L2 */

            LOG(DEBUG) << "Address " << addr << " was found in L2.";
            assert(L2.blockExists(addr) && L2.blockIsDirty(addr));

            // Insert the block fetched from L2 into L1
            if(config.policy){
                LOG(DEBUG) << "Write allocate is on!";
                LOG(DEBUG) << "Inserting address " << addr << " to L1.";
                // Write allocate is enabled
                L1.write(addr, false, false, &eviction_status, &write_back_addr);
                L1.setBlockDirty(addr);
                if(L2.blockIsDirty(addr)){
                    L2.setBlockNotDirty(addr);
                }
                if(eviction_status == WRITE_BACK){
                    // Evict block from L1 to L2
                    LOG(DEBUG) << "Address" << write_back_addr << " was evicted from L1 during the insertion.";
                    LOG(DEBUG) << "Writing address " << write_back_addr << " to L2.";
                    L2.write(write_back_addr, false, false, &eviction_status, &write_back_addr);
                    eviction_status = NOTHING;
                }
            }
        }
    }
    else{
        /** Block is in L1 */
        // Noting to be done
        LOG(DEBUG) << "Address " << addr << " was found in L1";
    }
}

SimStats MemoryManager::getStats() {
    SimStats ret = {0, 0, 0};
    Statistics l1_stats = L1.getStats();
    Statistics l2_stats = L2.getStats();
    unsigned total_misses_l1 = l1_stats.total_read_miss + l1_stats.total_write_miss;
    unsigned total_hits_l1 = l1_stats.total_read_hit + l1_stats.total_write_hit;
    ret.miss_rate_l1 = double(total_misses_l1) / (total_misses_l1 + total_hits_l1);

    unsigned total_misses_l2 = l2_stats.total_read_miss + l2_stats.total_write_miss;
    unsigned total_hits_l2 = l2_stats.total_read_hit + l2_stats.total_write_hit;
    ret.miss_rate_l2 = double(total_misses_l2) / (total_misses_l2 + total_hits_l2);

    ret.average_access_time = double(l1_stats.total_cycles + l2_stats.total_cycles + stats.memory_cycles) / (stats.total_writes + stats.total_reads);
    return ret;
}
