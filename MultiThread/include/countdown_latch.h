#ifndef COUNT_DOWN_LATCH_H
#define COUNT_DOWN_LATCH_H

#include <condition_variable>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

class CountDownLatch {
 public:
  explicit CountDownLatch(uint32_t count);

  void Await(uint32_t time_ms = 0);

  void CountDown();

  uint32_t GetCount() const; 

 private:
  uint32_t count_ = 0;
  mutable std::mutex mutex_;
  std::condition_variable cond_;
};

#endif