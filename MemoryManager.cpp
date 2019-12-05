#include "MemoryManager.h"


MemoryManager::MemoryManager(MMConfig cfg) : config(cfg),
                                             L1({config.block_size/config.l1_size, config.block_size,
                                                 config.l1_associativity, config.l1_cycle}),
                                             L2({config.block_size/config.l2_size, config.block_size,
                                                 config.l2_associativity, config.l2_cycle}), memory_cycles(0) {
    LOG(TRACE) << "MemoryManager constructed at " << this;
}

//void MemoryManager::read(unsigned addr) {
//    unsigned addr_to_write_back = 0;
//    unsigned tag = 0;
//    L1.addCycles();
//    if(!L1.read(addr)){
//        L2.addCycles();
//        // Block is not in L1
//        if(!L2.read(addr)){
//            // Block is not in L1 and not in L2
//            addr_to_write_back = L2.evict(addr);
//            if(addr_to_write_back){
//                // Mem cycles ++
//            }
//            L2.insertBlock(addr);
//            addr_to_write_back = L1.evict(addr);
//            if(addr_to_write_back){
//                L2.updateBlock(addr_to_write_back);
//            }
//            L1.insertBlock(addr);
//        }
//        else{
//            // Block is not in L1 but is in L2
//            addr_to_write_back = L1.evict(addr);
//            if(addr_to_write_back){
//                L2.updateBlock(addr_to_write_back);
//            }
//            L2.read(addr);
//            L1.insertBlock(addr);
//        }
//    }
//    else{
//        // Block is in L1 (and also in L2 - "INCLUSIVE")
//        // Nothing to be done
//    }
//}

void MemoryManager::read(unsigned addr) {
    bool write_back = false;
    unsigned write_back_addr = 0;

    if(!L1.read(addr, true, true, &write_back, &write_back_addr)){
        /** Block is not in L1 */

        write_back = false;

        if(!L2.read(addr, true, true, &write_back, &write_back_addr)){
            /** Block is not in L1 and not in L2 */

            write_back = false;

            // Fetch block from main memory
            this->memory_cycles += config.memory_cycles;

            // Insert the block fetched from main memory into L2
            L2.write(addr, false, true, &write_back, &write_back_addr);
            if(write_back){
                // Evict block from L2 to main memory
                this->memory_cycles += config.memory_cycles;
                write_back = false;
            }

            // Insert the block also to L1
            L1.write(addr, false, true, &write_back, &write_back_addr);
            if(write_back){
                // Evict block from L1 to L2
                L2.write(write_back_addr, false, true, &write_back, &write_back_addr);
                write_back = false;
            }
        }
        else{
            /** Block is not in L1 but is in L2 */

            // Insert the block fetched from L2 into L1
            L1.write(addr, false, true, &write_back, &write_back_addr);
            if(write_back){
                // Evict block from L1 to L2
                L2.write(write_back_addr, false, true, &write_back, &write_back_addr);
                write_back = false;
            }
        }

    }
    else{
        /** Block is in L1 */
        // Noting to be done
    }
}

//void MemoryManager::write(unsigned int addr) {
//    unsigned addr_to_write_back = 0;
//    unsigned tag = 0;
//    if(!L1.write(addr)){
//        // Block is not in L1
//        if(!L2.write(addr)){
//            // Block is not in L1 and not in L2
//            if(config.policy){
//                addr_to_write_back = L2.evict(addr);
//                if(addr_to_write_back){
//                    // Mem cycles ++
//                }
//                L2.insertBlock(addr);
//                addr_to_write_back = L1.evict(addr);
//                if(addr_to_write_back){
//                    L2.updateBlock(addr_to_write_back);
//                }
//                L1.insertBlock(addr);
//            }
//            else{
//                // Mem cycle ++
//            }
//        }
//        else{
//            // Block is not in L1 but is in L2
//            L2.updateBlock(addr);
//            if(config.policy){
//                addr_to_write_back = L1.evict(addr);
//                if(addr_to_write_back){
//                    L2.updateBlock(addr_to_write_back);
//                }
//                L2.read(addr);
//                L1.insertBlock(addr);
//            }
//        }
//    }
//    else{
//        // Block is in L1 (and also in L2 - "INCLUSIVE")
//        // Nothing to be done
//        L1.updateBlock(addr);
//    }
//}

void MemoryManager::write(unsigned int addr) {
    bool write_back = false;
    unsigned write_back_addr = 0;

    if(!L1.write(addr, true, true, &write_back, &write_back_addr)){ // If hit - automatically set dirty
        /** Block is not in L1 */

        write_back = false;

        if(!L2.write(addr, true, true, &write_back, &write_back_addr)){ // If hit - automatically set dirty
            /** Block is not in L1 and not in L2 */

            write_back = false;

            // Fetch block from main memory
            this->memory_cycles += config.memory_cycles;

            // Insert the block fetched from main memory into L2
            if(config.policy){
                L2.write(addr, false, true, &write_back, &write_back_addr);
                if(write_back){
                    // Evict block from L2 to main memory
                    this->memory_cycles += config.memory_cycles;
                    write_back = false;
                }

                // Insert the block also to L1
                L1.write(addr, false, true, &write_back, &write_back_addr);
                if(write_back){
                    // Evict block from L1 to L2
                    L2.write(write_back_addr, false, true, &write_back, &write_back_addr);
                    write_back = false;
                }
            }
        }
        else{
            /** Block is not in L1 but is in L2 */

            // Insert the block fetched from L2 into L1
            if(config.policy){
                // Write allocate is enabled
                L1.write(addr, false, true, &write_back, &write_back_addr);
                if(write_back){
                    // Evict block from L1 to L2
                    L2.write(write_back_addr, false, true, &write_back, &write_back_addr);
                    write_back = false;
                }
            }
        }
    }
    else{
        /** Block is in L1 */
        // Noting to be done
    }
}