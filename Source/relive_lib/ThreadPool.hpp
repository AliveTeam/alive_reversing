#pragma once

#include <thread>
#include <functional>
#include <vector>
#include <queue>
#include <atomic>
#include <condition_variable>

class IJob
{
public:
    virtual ~IJob() = default;
    virtual void Execute() = 0;
};

using UP_IJob = std::unique_ptr<IJob>;

// Note: This thread pool will abondon yet to be run jobs when it
// goes out of scope.
class ThreadPool final
{
public:

    ThreadPool()
    {
        // Spawn as many threads as CPU cores (usually, can be less or more depending on the impl)
        for (u32 i = 0; i < std::thread::hardware_concurrency(); i++)
        {
            mThreads.emplace_back(std::thread(&ThreadPool::ThreadProc, this));
        }
    }

    [[nodiscard]] bool Busy() const
    {
        bool isBusy = false;
        {
            std::unique_lock lock(mJobsQueueMutex);
            isBusy = !mJobs.empty() || mBusyJobs != 0;
        }
        return isBusy;
    }

    ~ThreadPool()
    {
        // Threads will read this flag when they wake up
        {
            std::unique_lock lock(mJobsQueueMutex);
            mStopThreads = true;
        }

        // Wake them all
        mWaitForWork.notify_all();

        // Wait on them to stop
        for (auto& thread : mThreads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
        mThreads.clear();
    }

    void AddJob(UP_IJob job)
    {
        if (!job)
        {
            // Actually a fatal error
            return;
        }

        {
            std::unique_lock lock(mJobsQueueMutex);
            mJobs.emplace(std::move(job));
        }
        mWaitForWork.notify_one();
    }

private:
    void ThreadProc()
    {
        for (;;)
        {
            UP_IJob job;
            {
                std::unique_lock<std::mutex> lock(mJobsQueueMutex);
                mWaitForWork.wait(lock, [this]()
                                  { return !mJobs.empty() || mStopThreads; });

                if (mStopThreads)
                {
                    return;
                }

                if (!mJobs.empty())
                {
                    job = std::move(mJobs.front());
                    mJobs.pop();
                    ++mBusyJobs;
                }
            }
            job->Execute();
            --mBusyJobs;
        }
    }

    std::atomic<int> mBusyJobs{0};
    std::atomic<bool> mStopThreads{false};
    mutable std::mutex mJobsQueueMutex;
    std::condition_variable mWaitForWork;
    std::vector<std::thread> mThreads;
    std::queue<UP_IJob> mJobs;
};
