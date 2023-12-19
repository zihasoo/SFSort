
#include "thread_pool.h"

thread_pool::thread_pool() : thread_pool(std::thread::hardware_concurrency()) {}

thread_pool::thread_pool(unsigned threads_count) : num_threads(threads_count) {
    workers.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        workers.emplace_back([this]() { this->worker_function(); });
    }
}

thread_pool::~thread_pool() {
    stop_all = true;
    job_q_cv.notify_all();

    for (auto &w: workers) {
        w.join();
    }
}

void thread_pool::register_job(std::function<void()> job) {
    //if (stop_all) return;
    job_q_mutex.lock();
    jobs.push(std::move(job));
    job_q_mutex.unlock();
    job_q_cv.notify_one();
}

void thread_pool::worker_function() {
    while (true) {
        std::unique_lock<std::mutex> lock(job_q_mutex);
        job_q_cv.wait(lock, [this]() { return !this->jobs.empty() || stop_all; });
        if (stop_all && this->jobs.empty())
            return;

        auto job = std::move(jobs.front());
        jobs.pop();
        lock.unlock();

        job();
    }
}
