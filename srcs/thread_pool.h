
#ifndef SFSORT_THREAD_POOL_H
#define SFSORT_THREAD_POOL_H

#include <thread>
#include <functional>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <vector>

class thread_pool {
public:
    thread_pool();

    explicit thread_pool(unsigned num_threads);

    ~thread_pool();

    void register_job(std::function<void()> job);

private:
    unsigned num_threads;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;
    std::condition_variable job_q_cv;
    std::mutex job_q_mutex;

    bool stop_all = false;

    void worker_function();
};


#endif //SFSORT_THREAD_POOL_H
