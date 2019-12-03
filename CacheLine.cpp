
#include "CacheLine.h"
#include "Logger.h"

CacheLine::CacheLine(unsigned block_size): valid(false), dirty(false), tag(0), block_address(0), block_size(block_size) {
    LOG(TRACE) << "CacheLine constructed at " << this;
}

CacheLine::CacheLine(const CacheLine &cache_line) {
    LOG(TRACE) << "CacheLine constructed(*copy*) at " << this;
    this->block_size = cache_line.block_size;
    this->block_address = cache_line.block_address;
    this->tag = cache_line.tag;
    this->valid = cache_line.valid;
    this->dirty = cache_line.dirty;
}

bool CacheLine::isValid() {
    return this->valid;
}

unsigned CacheLine::getTag() {
    return this->tag;
}

bool CacheLine::isDirty() {
    return this->dirty;
}

unsigned CacheLine::getBlockAddress() {
    return this->block_address;
}

void CacheLine::invalidate() {
    this->valid = false;
}

void CacheLine::validate() {
    this->valid = true;
}

void CacheLine::setTag(unsigned tag) {
    this->tag = tag;
}

void CacheLine::setBlockAddress(unsigned addr) {
    this->block_address = addr;
}

void CacheLine::setDirty() {
    this->dirty = true;
}

void CacheLine::setNotDirty() {
    this->dirty = false;
}
