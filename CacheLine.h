
#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_CACHELINE_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_CACHELINE_H


class CacheLine {
private:
    bool valid;
    unsigned tag;
    unsigned block_start_addr;
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
    unsigned getTag();
};


#endif //COMPUTER_ARCHITECTURE_HW2_CACHE_CACHELINE_H
