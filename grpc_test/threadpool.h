/* Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
//#include "glog/logging.h"
//#include "paddle/fluid/platform/enforce.h"
//#include "paddle/fluid/platform/macros.h"  // for DISABLE_COPY_AND_ASSIGN
#include <sys/time.h>
#include <iostream>
#include <string>
#include <thread>
#include <sstream>

#include <dlfcn.h>     // for dladdr
#include <execinfo.h>  // for backtrace
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include "tinyformat.h"

namespace paddle {
namespace framework {

template <typename... Args>
void Fprintf(std::ostream& out, const char* fmt, const Args&... args) {
    paddle::string::tinyformat::vformat(out, fmt, paddle::string::tinyformat::makeFormatList(args...));
}

template <typename... Args>
std::string Sprintf(const char* fmt, const Args&... args) {
  std::ostringstream oss;
  Fprintf(oss, fmt, args...);
  return oss.str();
}

template <typename... Args>
void Printf(const char* fmt, const Args&... args) {
  Fprintf(std::cout, fmt, args...);
}

inline std::string demangle(std::string name) { return name; }

#ifndef DISABLE_COPY_AND_ASSIGN
#define DISABLE_COPY_AND_ASSIGN(classname)         \
 private:                                          \
  classname(const classname&) = delete;            \
  classname(classname&&) = delete;                 \
  classname& operator=(const classname&) = delete; \
  classname& operator=(classname&&) = delete
#endif

struct EnforceNotMet : public std::exception {
    std::exception_ptr exp_;
    std::string err_str_;
    EnforceNotMet(std::exception_ptr e, const char* f, int l) : exp_(e) {
      static constexpr int TRACE_STACK_LIMIT = 100;
      try {
        std::rethrow_exception(exp_);
      } catch (const std::exception& exp) {
        std::ostringstream sout;
     sout << Sprintf("%s at [%s:%d]", exp.what(), f, l) << std::endl;
        sout << "PaddlePaddle Call Stacks: " << std::endl;

        void* call_stack[TRACE_STACK_LIMIT];
        auto size = backtrace(call_stack, TRACE_STACK_LIMIT);
        auto symbols = backtrace_symbols(call_stack, size);

        Dl_info info;
        for (int i = 0; i < size; ++i) {
          if (dladdr(call_stack[i], &info) && info.dli_sname) {
            auto demangled = demangle(info.dli_sname);
            auto addr_offset = static_cast<char*>(call_stack[i]) -
                               static_cast<char*>(info.dli_saddr);
          sout << Sprintf("%-3d %*0p %s + %zd\n", i,
                                    2 + sizeof(void*) * 2, call_stack[i],
                                    demangled, addr_offset);
          } else {
          sout << Sprintf("%-3d %*0p\n", i, 2 + sizeof(void*) * 2,
                                    call_stack[i]);
          }
        }
        free(symbols);
        err_str_ = sout.str();
      }
    }

    const char* what() const noexcept { return err_str_.c_str(); }
  };

// ThreadPool maintains a queue of tasks, and runs them using a fixed
// number of threads.
class ThreadPool {
 public:
  using Task = std::packaged_task<std::unique_ptr<EnforceNotMet>()>;

  // Returns the singleton of ThreadPool.
  static ThreadPool* GetInstance();

  ~ThreadPool();

  // Returns the number of threads created by the constructor.
  size_t Threads() const { return total_threads_; }

  // Returns the number of currently idle threads.
  size_t IdleThreads() {
    std::unique_lock<std::mutex> lock(mutex_);
    return idle_threads_;
  }

  // Run pushes a function to the task queue and returns a std::future
  // object.  To wait for the completion of the task, call
  // std::future::wait().
  template <typename Callback>
  std::future<void> Run(Callback fn) {
    auto f = this->RunAndGetException(fn);
    return std::async(std::launch::deferred, ExceptionHandler(std::move(f)));
  }

  template <typename Callback>
  std::future<std::unique_ptr<EnforceNotMet>> RunAndGetException(
      Callback fn) {
    std::unique_lock<std::mutex> lock(mutex_);
    Task task([fn]() -> std::unique_ptr<EnforceNotMet> {
      try {
        fn();
      } catch (EnforceNotMet ex) {
        return std::unique_ptr<EnforceNotMet>(
            new EnforceNotMet(ex));
      } catch (const std::exception& e) {
      std::cout << "Unexpected exception is catched in thread pool. All "
                      "throwable exception in Fluid should be an EnforceNotMet."
                   << e.what();
                   exit(1);
      }
      return nullptr;
    });
    std::future<std::unique_ptr<EnforceNotMet>> f = task.get_future();
    tasks_.push(std::move(task));
    lock.unlock();
    scheduled_.notify_one();
    return f;
  }

  // Wait until all the tasks are completed.
  void Wait();

 private:
  struct ExceptionHandler {
    mutable std::future<std::unique_ptr<EnforceNotMet>> future_;
    explicit ExceptionHandler(
        std::future<std::unique_ptr<EnforceNotMet>>&& f)
        : future_(std::move(f)) {}
    void operator()() const {
      auto ex = this->future_.get();
      if (ex != nullptr) {
          std::cout << "The exception is thrown inside the thread pool. You "
                      "should use RunAndGetException to handle the exception.\n"
                      "The default exception handler is LOG(FATAL)."
                   << ex->what();
          exit(1);
      }
    }
  };

  DISABLE_COPY_AND_ASSIGN(ThreadPool);

  explicit ThreadPool(int num_threads);

  // If the task queue is empty and avaialbe is equal to the number of
  // threads, means that all tasks are completed.  Note: this function
  // is not thread-safe.  Returns true if all tasks are completed.
  // Note: don't delete the data member total_threads_ and use
  // threads_.size() instead; because you'd need to lock the mutex
  // before accessing threads_.
  bool Done() { return tasks_.empty() && idle_threads_ == total_threads_; }

  // The constructor starts threads to run TaskLoop, which retrieves
  // and runs tasks from the queue.
  void TaskLoop();

  // Init is called by GetInstance.
  static void Init();

 private:
  static std::unique_ptr<ThreadPool> threadpool_;
  static std::once_flag init_flag_;

  std::vector<std::unique_ptr<std::thread>> threads_;
  const size_t total_threads_;
  size_t idle_threads_;

  std::queue<Task> tasks_;
  std::mutex mutex_;
  bool running_;
  std::condition_variable scheduled_;
  std::condition_variable completed_;
};

// Run a function asynchronously.
// NOTE: The function must return void. If the function need to return a value,
// you can use lambda to capture a value pointer.
template <typename Callback>
std::future<void> Async(Callback callback) {
  return ThreadPool::GetInstance()->Run(callback);
}

}  // namespace framework
}  // namespace paddle
