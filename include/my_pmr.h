#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <memory_resource>
#include <iostream>
#include <new>
#include <stdexcept>


class LinearMemSource : public std::pmr::memory_resource {
public:
    LinearMemSource();
    explicit LinearMemSource(size_t buf);
    ~LinearMemSource() override;

    LinearMemSource(const LinearMemSource&) = delete;
    LinearMemSource& operator=(const LinearMemSource&) = delete;

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void  do_deallocate(void* ptr, size_t bytes, size_t alignment) override;
    bool  do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

private:

    struct LinearBuffer {
        uint8_t* mem;       /*!< Pointer to buffer memory. */
        int totalSize; /*!< Total size in bytes. */
        int offset;    /*!< Offset. */
    } buffer;

    bool ownsMemory = false;

    struct Block {
        void* ptr;
        size_t size;
        bool in_use;
    };

    std::vector<Block> my_blocks; 

};

