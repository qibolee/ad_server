#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <mutex>
#include <utility>
#include <iostream>
#include <condition_variable>




template<typename T>
class blocking_queue {
    using uint = unsigned int;

public:
    /**
     * constructor with capacity
     */
    blocking_queue(uint size = 1024);

    blocking_queue(const blocking_queue &bq) = delete;
    blocking_queue(blocking_queue &&bq) = delete;
    blocking_queue &operator=(const blocking_queue &bq) = delete;
    blocking_queue &operator=(blocking_queue &&bq) = delete;

    /**
     * destroy
     */
    ~blocking_queue();

    /**
     * get a num, put in val
     * return 0 on success, or -1 if error occurred
     */
    void get(T &val);

    /**
     * put a num by val
     * return 0 on success, or -1 if error occurred
     */
    void put(const T &val);

    /**
     * get queue capacity
     */
    uint capacity() const { return siz; }

    /**
     * get queue size
     */
    uint size() const { return end >= beg ? end - beg : end - beg + siz; }

private:
    static std::pair<uint, uint> range_size;
    std::mutex mutex;
    std::condition_variable cond;
    T *queue;
    uint beg;
    uint end;
    uint siz;
    bool is_empty() const { return beg == end; }
    bool is_full() const { return ((end + 1) % siz) == beg; }
};

template<typename T>
std::pair<unsigned, unsigned> blocking_queue<T>::range_size = std::make_pair<unsigned, unsigned>(8, 1024);

template<typename T>
blocking_queue<T>::blocking_queue(uint sz): queue(nullptr), beg(0), end(0), siz(sz) {

    if (siz < range_size.first) {
        siz = range_size.first;
    } else if (siz > range_size.second) {
        siz = range_size.second;
    }
    queue = new T[siz];
}

template<typename T>
void blocking_queue<T>::get(T &val) {

    std::unique_lock<std::mutex> lk(mutex);
    cond.wait(lk, [&]{ return !this->is_empty(); });
    val = queue[beg];
    beg = (beg + 1) % siz;
    cond.notify_all();
}

template<typename T>
void blocking_queue<T>::put(const T &val) {

    std::unique_lock<std::mutex> lk(mutex);
    cond.wait(lk, [&]{ return !this->is_full(); });
    queue[siz] = val;
    beg = (beg + 1) % siz;
    cond.notify_all();
}

template<typename T>
blocking_queue<T>::~blocking_queue() {
    delete [] queue;
}


#endif //BLOCKING_QUEUE_H
