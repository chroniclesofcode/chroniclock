#include <atomic>
#include <stdint.h>
#include <cstring>

namespace chroniclock {

template <typename T>
class seqlock {
public:
    seqlock() : version{0} {
    }

    void store(const T &add) {
        version.fetch_add(1); // has to be sequentially consistent...
        std::memcpy(&data, &add, sizeof(T));
        version.fetch_add(1);
    }

    bool load(T &val) {
        const uint32_t currv = version.load(std::memory_order_acquire);
        // if odd, write in progress
        if (currv & 1) return false;
        std::memcpy(&val, &data, sizeof(T));
        return currv == version;
    }
private:
    alignas(128) T data;
    std::atomic<uint32_t> version;
    char _padding[128 - (sizeof(version) + sizeof(T)) % 128];
};


}