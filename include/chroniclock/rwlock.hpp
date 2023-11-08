#include <thread>
#include <mutex>
#include <condition_variable>

namespace chroniclock {

template<typename T>
class rwlock {
public: 

    void read_lock() {
        std::unique_lock<std::mutex> lock(mtx);
        readers_waiting.wait(lock, [this] { return !writing && writer_q == 0; });
        reader_ct++;
    }

    void read_unlock() {
        std::unique_lock<std::mutex> lock(mtx);
        reader_ct--;
        if (reader_ct == 0 && writer_q > 0) {
            lock.unlock();
            writers_waiting.notify_one();
        }
    }

    void write_lock() {
        std::unique_lock<std::mutex> lock(mtx);
        writer_q++;
        writers_waiting.wait(lock, [this]{ return !writing && reader_ct == 0; });
        writer_q--;
        writing = true;
    }

    void write_unlock() {
        std::unique_lock<std::mutex> lock(mtx);
        if (writer_q > 0) {
            lock.unlock();
            writers_waiting.notify_one();
        } else {
            lock.unlock();
            writing = false;
            readers_waiting.notify_all();
        }
    }

    void store(const T &add) {
        write_lock();
        std::memcpy(&data, &add, sizeof(T));
        write_unlock();
    }

    bool load(T &val) {
        read_lock();
        std::memcpy(&val, &data, sizeof(T));
        read_unlock();
        return true;
    }

private:
    alignas(128) T data;
    alignas(128) std::mutex mtx;
    std::condition_variable readers_waiting;
    std::condition_variable writers_waiting;
    int reader_ct = 0;
    int writer_q = 0;
    bool writing = false;
};

}
