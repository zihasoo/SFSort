#include "thread_pool.h"

thread_pool::thread_pool(int num_threads) : num_threads(num_threads) {
    workers.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
//        workers.emplace_back(worker_function);
    }
}

thread_pool::~thread_pool() {
    stop_all = true;
    cv_job_q.notify_all();
}
