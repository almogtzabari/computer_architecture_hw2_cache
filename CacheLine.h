
#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_CACHELINE_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_CACHELINE_H

/**
 * A memory block class.
 */
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

    /**
     * Determines if the block is valid (The block is not valid if it was evicted from lower levels or it hasn't changed since it has been initialized).
     * @return - True if the block is valid, else false.
     */
    bool isValid();

    /**
     * Sets a block to invalid.
     */
    void invalidate();

    /**
     * Sets a block to valid.
     */
    void validate();

    /**
     * Determines if the block is dirty ( A block is dirty if it has been modified since it was brought to the cache).
     * @return - True if a block is dirty, else false.
     */
    bool isDirty();

    /**
     * Sets a block as dirty (The block has been modified).
     */
    void setDirty();

    /**
     * Sets a block as not dirty.
     */
    void setNotDirty();

    /**
     * Returns a block's memory address.
     * @return - A block's memory address.
     */
    unsigned getBlockAddress();

    /**
     * Returns a block's tag.
     * @return - A block's tag.
     */
    unsigned getTag();

    /**
     * Sets a block's tag.
     * @param tag - A given block's tag.
     */
    void setTag(unsigned tag);

    /**
     * Sets a block's address.
     * @param addr - A given block's address.
     */
    void setBlockAddress(unsigned addr);
};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_CACHELINE_H
