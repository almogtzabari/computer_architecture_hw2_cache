
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
