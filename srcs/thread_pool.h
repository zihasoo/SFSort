
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
    thread_pool(int num_threads);

    ~thread_pool();

private:
    int num_threads;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> jobs;
    std::condition_variable cv_job_q;
    std::mutex m_job_q;

    bool stop_all = false;

    void worker_function();
};


#endif //SFSORT_THREAD_POOL_H
