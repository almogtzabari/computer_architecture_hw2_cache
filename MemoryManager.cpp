#include "MemoryManager.h"


MemoryManager::MemoryManager(MMConfig cfg) : config(cfg),
                                             L1({config.block_size/config.l1_size, config.block_size,
                                                 config.l1_associativity, config.l1_cycle}),
                                             L2({config.block_size/config.l2_size, config.block_size,
                                                 config.l2_associativity, config.l2_cycle}) {
    LOG(TRACE) << "MemoryManager constructed at " << this;
}

void MemoryManager::read(unsigned addr) {
    if(!L1.read(addr)){

    }
}

