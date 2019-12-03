#include "MemoryManager.h"


MemoryManager::MemoryManager(MMConfig cfg) : config(cfg),
                                             L1({config.block_size/config.l1_size, config.block_size,
                                                 config.l1_associativity, config.l1_cycle}),
                                             L2({config.block_size/config.l2_size, config.block_size,
                                                 config.l2_associativity, config.l2_cycle}) {
    LOG(TRACE) << "MemoryManager constructed at " << this;
}

void MemoryManager::read(unsigned addr) {
    unsigned addr_to_write_back = 0;
    unsigned tag = 0;
    L1.addCycles();
    if(!L1.read(addr)){
        L2.addCycles();
        // Block is not in L1
        if(!L2.read(addr)){
            // Block is not in L1 and not in L2
            addr_to_write_back = L2.evict(addr);
            if(addr_to_write_back){
                // Mem cycles ++
            }
            L2.insertBlock(addr);
            addr_to_write_back = L1.evict(addr);
            if(addr_to_write_back){
                L2.updateBlock(addr_to_write_back);
            }
            L1.insertBlock(addr);
        }
        else{
            // Block is not in L1 but is in L2
            addr_to_write_back = L1.evict(addr);
            if(addr_to_write_back){
                L2.updateBlock(addr_to_write_back);
            }
            L2.read(addr);
            L1.insertBlock(addr);
        }
    }
    else{
        // Block is in L1 (and also in L2 - "INCLUSIVE")
        // Nothing to be done
    }
}

void MemoryManager::write(unsigned int addr) {
    unsigned addr_to_write_back = 0;
    unsigned tag = 0;
    if(!L1.write(addr)){
        // Block is not in L1
        if(!L2.write(addr)){
            // Block is not in L1 and not in L2
            if(config.policy){
                addr_to_write_back = L2.evict(addr);
                if(addr_to_write_back){
                    // Mem cycles ++
                }
                L2.insertBlock(addr);
                addr_to_write_back = L1.evict(addr);
                if(addr_to_write_back){
                    L2.updateBlock(addr_to_write_back);
                }
                L1.insertBlock(addr);
            }
            else{
                // Mem cycle ++
            }
        }
        else{
            // Block is not in L1 but is in L2
            L2.updateBlock(addr);
            if(config.policy){
                addr_to_write_back = L1.evict(addr);
                if(addr_to_write_back){
                    L2.updateBlock(addr_to_write_back);
                }
                L2.read(addr);
                L1.insertBlock(addr);
            }
        }
    }
    else{
        // Block is in L1 (and also in L2 - "INCLUSIVE")
        // Nothing to be done
        L1.updateBlock(addr);
    }
}

