//
// Created by Almog on 02/12/2019.
//

#include "CacheLine.h"
#include "Logger.h"

CacheLine::CacheLine(unsigned block_size): valid(false), tag(0), block_start_addr(0), block_size(block_size) {
    LOG(TRACE) << "CacheLine constructed at " << this;
}

CacheLine::CacheLine(const CacheLine &cache_line) {
    LOG(TRACE) << "CacheLine constructed(*copy*) at " << this;
    this->block_size = cache_line.block_size;
    this->block_start_addr = cache_line.block_start_addr;
    this->tag = cache_line.tag;
    this->valid = cache_line.valid;
}

bool CacheLine::isValid() {
    return valid;
}

unsigned CacheLine::getTag() {
    return tag;
}
