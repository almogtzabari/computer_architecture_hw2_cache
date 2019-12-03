
#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_CACHELINE_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_CACHELINE_H


class CacheLine {
private:
    bool valid;
    bool dirty;
    unsigned tag;
    unsigned block_address;
    unsigned block_size;

public:
    /**
     * Constructor
     * @param block_size - size of block in bytes.
     */
    CacheLine(unsigned block_size);

    /**
     * Copy constructor
     * @param cache_line - cache line to copy
     */
    CacheLine(const CacheLine& cache_line);

    bool isValid();
    void invalidate();
    void validate();
    bool isDirty();
    void setDirty();
    void setNotDirty();
    unsigned getBlockAddress();
    unsigned getTag();
    void setTag(unsigned tag);
    void setBlockAddress(unsigned addr);
};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_CACHELINE_H
