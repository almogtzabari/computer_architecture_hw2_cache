
#include <assert.h>
#include "Set.h"
#include "Logger.h"

Set::Set(unsigned associativity, unsigned block_size) : lines(associativity, CacheLine(block_size)),
                                                        ranking(associativity) {
    unsigned i = 0;
    for(auto& num: ranking){
        num = i++;
    }
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
    assert(!lines[ranking[ranking.size()-1]].isValid());
    CacheLine& insert_into = lines[ranking[ranking.size()-1]];
    insert_into.setTag(tag);
    insert_into.setBlockAddress(addr);
    insert_into.validate();
    insert_into.setNotDirty();
}

void Set::setDirty(unsigned tag) {
    assert(this->blockExists(tag));
    for (auto& line: lines){
        if(line.getTag() == tag){
            line.setDirty();
        }
    }
}

void Set::updateLRU(unsigned tag) {
    assert(blockExists(tag));
    unsigned i;
    for (i = 0; i < ranking.size(); i++){
        if(lines[ranking[i]].getTag() == tag && lines[ranking[i]].isValid()){
            break;
        }
    }
    unsigned fresh_one = ranking[i];
    for (unsigned j = i; j > 0; j--){
        ranking[j] = ranking[j-1];
    }
    ranking[0] = fresh_one;
}
