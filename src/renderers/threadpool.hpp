#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <cassert>

namespace CT
{
class ThreadPool 
{
public:
    ThreadPool(size_t);

    // Enque a task to be executed by the thread pool
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    ~ThreadPool();

private:
    
    // Vector to store all worker threads
    std::vector<std::thread> workers;
    
    // Queue of all tasks to be executed
    std::queue<std::function<void()>> tasks;
    
    // Mutex to protect access to the queue
    std::mutex queue_mutex;

    // Condition variable to notify threads when tasks are available
    std::condition_variable condition;

    // Flag to indicate when the pool is being destroyed
    bool stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads) : stop(false)
{
    // Create the specified number of worker threads
    for(size_t i = 0; i < threads; i++)
    {
        // Each thread is launched with a lambda function
        workers.emplace_back( [this]
        {
            // Loop until the pool is complete
            while(true)
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);

                    // Wait until there is a task to be executed
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

                    // If the pool is complete and there are no more tasks, exit
                    if(this->stop && this->tasks.empty())
                        return;

                    // Get the next task from the queue
                    task = std::move(this->tasks.front());

                    // Remove the completed task from the queue
                    this->tasks.pop();
                }

                // Execute the task
                task();
            }
        });
    }
}


template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
{
    // Get the return type of the function F
    using return_type = typename std::result_of<F(Args...)>::type;

    // Create a shared pointer 'task' to wrap the function
    auto task = std::make_shared<std::packaged_task<return_type()>>
        (std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    
    // Get the future from the task
    std::future<return_type> ret = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // Enqueuing is illegal if the pool is stopped
        assert(!stop);

        // Add the task to the queue
        tasks.emplace([task](){ (*task)(); });
    }

    // Notify a thread that a task is available
    condition.notify_one();

    return ret;
}

// Destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // Set the stop flag to true
        stop = true;
    }

    // Notify all threads that the pool is complete
    condition.notify_all();

    // Join all threads and wait for them to complete
    for(std::thread &worker: workers)
        worker.join();
}

}