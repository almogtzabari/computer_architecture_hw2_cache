
#include "Set.h"
#include "Logger.h"

Set::Set(unsigned associativity, unsigned block_size) : lines(associativity, CacheLine(block_size)),
                                                        ranking(associativity) {
    LOG(TRACE) << "Set constructed at " << this;
}

bool Set::blockExists(unsigned tag) {
    for (auto& line: lines){
        if(line.isValid() && line.getTag() == tag){
            return true;
        }
    }
    return false;
}

unsigned Set::evict() {
    CacheLine& line_to_evict = lines[ranking[ranking.size()-1]];
    unsigned addr_to_write_back = 0;
    if(line_to_evict.isValid() && line_to_evict.isDirty()){
        addr_to_write_back = line_to_evict.getBlockAddress();
    }
    line_to_evict.invalidate();
    return addr_to_write_back;
}

void Set::insertBlock(unsigned tag, unsigned addr) {
    CacheLine& insert_into = lines[ranking[ranking.size()-1]];
    insert_into.setTag(tag);
    insert_into.setBlockAddress(addr);
    insert_into.validate();
    insert_into.setNotDirty();
    // Update rankings accordingly
}
