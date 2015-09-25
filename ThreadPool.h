//
// Created by William Lees on 9/25/15.
//

#ifndef OSPROJ1_THREADPOOL_H
#define OSPROJ1_THREADPOOL_H

#include <queue>
#include <sys/_pthread/_pthread_mutex_t.h>
#include <sys/_pthread/_pthread_t.h>

#define         THREAD_POOL_SIZE            10

struct Task {
    void *fn;
    void *args;
};

class ThreadPool {
public:
    /**
     * Returns the singleton instance of the thread pool
     */
    static ThreadPool * const getThreadPool();

    /**
     * Adds a task to the current task queue waiting
     * for a free thread
     *
     * @param task is a pointer to the function that represents the task to execute
     */
    void addTask(void *task);

private:
    // Singleton instance
    static ThreadPool *m_sInstance;

    // Queue of tasks (pointers to functions)
    std::queue<Task> m_Tasks;
    pthread_mutex_t m_TasksMutex;

    // Threads
    pthread_t m_Threads[THREAD_POOL_SIZE];

    // Constructor
    ThreadPool();

    // Function that each thread runs
    static void *runThread(void* args);
};


#endif //OSPROJ1_THREADPOOL_H
