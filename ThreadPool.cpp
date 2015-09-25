//
// Created by William Lees on 9/25/15.
//

#include "ThreadPool.h"

#include <thread>

ThreadPool::ThreadPool() {
    // Generate the threads
    for (unsigned char i = 0; i < THREAD_POOL_SIZE; ++i) {
        pthread_create(&this->m_Threads[i], nullptr, runThread, nullptr);
    }
}

void *ThreadPool::runThread(void *args) {
    ThreadPool const *threadPool = ThreadPool::getThreadPool();
    for(;;) {
        // There is nothing waiting in the task queue
        if (threadPool->m_Tasks.size() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }

        // There are tasks waiting in the queue
        else {
            // Get the next task in the queue
            pthread_mutex_lock(&threadPool->m_TasksMutex);
            Task task = threadPool->m_Tasks.front();
            threadPool->m_Tasks.pop();
            pthread_mutex_unlock(&threadPool->m_TasksMutex);

            // Execute the task
            ((void (*)(void *)) task.fn)(task.args);


        }
    }
}

ThreadPool * const ThreadPool::getThreadPool() {
    if (ThreadPool::m_sInstance == nullptr) {
        ThreadPool::m_sInstance = new ThreadPool();
    }

    return ThreadPool::m_sInstance;
}

void ThreadPool::addTask(void *task) {
    this->m_Tasks.push(task);
}