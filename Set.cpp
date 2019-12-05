
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

bool Set::evict(unsigned* write_back_addr) {
    CacheLine& line_to_evict = lines[ranking[ranking.size()-1]];
    if(line_to_evict.isValid() && line_to_evict.isDirty()){
        // Block needs to be evicted
        *write_back_addr = line_to_evict.getBlockAddress();
        line_to_evict.invalidate();
        return true;
    }
    else{
        // Block does not need to be evicted
        line_to_evict.invalidate();
        return false;
    }
}

void Set::insertBlock(unsigned tag, unsigned addr) {
    CacheLine& insert_into = lines[ranking[ranking.size()-1]];
    insert_into.setTag(tag);
    insert_into.setBlockAddress(addr);
    insert_into.validate();
    insert_into.setNotDirty();
}
