
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

EVICTION_STATUS Set::evict(unsigned* write_back_addr) {
    CacheLine& line_to_evict = lines[ranking[ranking.size()-1]];
    if(line_to_evict.isValid()){
        line_to_evict.invalidate();
        *write_back_addr = line_to_evict.getBlockAddress();
        if(line_to_evict.isDirty()){
            return WRITE_BACK;
        }
        else{
            return INVALIDATE;
        }
    }
    else{
        return NOTHING;
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

void Set::invalidate(unsigned tag) {
    assert(this->blockExists(tag));
    for(unsigned i = 0; i < ranking.size(); i++) {
        if (lines[ranking[i]].getTag() == tag && lines[ranking[i]].isValid()) {
            lines[ranking[i]].invalidate();
            unsigned old = ranking[i];
            for (unsigned j = i; j < ranking.size()-1; j++) {
                ranking[j] = ranking[j + 1];
            }
            ranking[ranking.size()-1] = old;
            return;
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
