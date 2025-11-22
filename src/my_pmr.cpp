#include "my_pmr.h"


LinearMemSource::LinearMemSource() {
    size_t defaultSize = 1024 * 1024; 
    buffer.mem = new uint8_t[defaultSize];
    buffer.totalSize = static_cast<int>(defaultSize);
    buffer.offset = 0;
    ownsMemory = true;
}

LinearMemSource::LinearMemSource(size_t bufSize = 1024 * 1024) {
        buffer.mem = new uint8_t[bufSize];
        buffer.totalSize = bufSize;
        buffer.offset = 0;
        ownsMemory = true;

        std::cout << "LinearMemSource created, buffer size = " << bufSize << " bytes\n";
    }

LinearMemSource::~LinearMemSource() {
        if (ownsMemory)
            delete[] buffer.mem;

        std::cout << "LinearMemSource destroyed\n";
    }

void* LinearMemSource:: do_allocate(size_t bytes, size_t alignment) {
        for (auto& block : my_blocks) {
            if (!block.in_use && block.size >= bytes) {
                block.in_use = true;
                return block.ptr;
            }
        }
        if (buffer.offset + static_cast<int>(bytes) > buffer.totalSize)
            throw std::bad_alloc();
        uint8_t* ptr = buffer.mem + buffer.offset;
        buffer.offset += static_cast<int>(bytes);
        my_blocks.push_back({ptr, bytes, true});

        return ptr;
    }

void LinearMemSource::do_deallocate(void* ptr, size_t bytes, size_t alignment) {
        for (auto& block : my_blocks) {
            if (block.ptr == ptr) {
                if (!block.in_use)
                    throw std::logic_error("Double deallocation detected");
                block.in_use = false;
                return;
            }
        }
        throw std::logic_error("Pointer does not belong to this LinearMemSource");
    }

bool LinearMemSource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
        return this == &other;
}