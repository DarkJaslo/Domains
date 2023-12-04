#ifndef ThreadPool_h
#define ThreadPool_h

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

namespace jaslo
{

//TODO: dependencies / data protection
class ThreadPool
{
public:

  ThreadPool(size_t nThreads) : _stop(false) 
  {
    _runningThreads = 0;

    for (size_t i = 0; i < nThreads; ++i) 
    {
      _threads.emplace_back(
        [this] {
          while (true) 
          {
            std::function<void()> task;
            {
              std::unique_lock<std::mutex> lock(_queueMutex);

              //Wait until stop or until there is a task
              _condition.wait(lock, [this] { return _stop or not _tasks.empty(); });

              //Stopped, return if there are no tasks left
              if (_stop and _tasks.empty()) 
                return;

              //Get the task from the queue (removes it because it moves)
              task = std::move(_tasks.front());
              {
                std::unique_lock<std::mutex> lock(_runningMutex);
                _runningThreads++;
              }
              _tasks.pop();
            }

            //Execution of the task
            task();

            //Task has finished, decrease number of running threads
            {
              std::unique_lock<std::mutex> lock(_runningMutex);
              _runningThreads--;
            }
          }
        }
      );
    }
  }

  template<class F, class... Args>
  auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))> 
  {
    using return_type = decltype(f(args...));
    auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(_queueMutex);
      if (_stop)
        throw std::runtime_error("enqueue on stopped ThreadPool");
      _tasks.emplace([task]() { (*task)(); });
    }
    _condition.notify_one();
    return res;
  }

  ~ThreadPool()
  {
    {
      std::unique_lock<std::mutex> lock(_queueMutex);
      _stop = true;
    }

    _condition.notify_all();

    for(std::thread& th : _threads)
    {
      th.join();
    }
  }

  bool running() const
  {
    return _runningThreads + _tasks.size() > 0;
  }

private:

  std::vector<std::thread> _threads;
  std::queue<std::function<void()>> _tasks;
  int _runningThreads; //Scales poorly

  std::mutex _queueMutex;
  std::mutex _runningMutex;
  std::condition_variable _condition;
  bool _stop;
};

}; //Namespace end


#endif