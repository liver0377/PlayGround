#include "countdown_latch.h"

CountDownLatch::CountDownLatch(uint32_t count) : count_(count) {}

void CountDownLatch::Await(uint32_t time_ns) {
  std::unique_lock<std::mutex> lock(mutex_);

  while (count_ > 0) {
    if (time_ns) {
      cond_.wait_for(lock, std::chrono::nanoseconds(time_ns));
    } else {
      cond_.wait(lock);
    }
  }
}

void CountDownLatch::CountDown() {
  std::lock_guard<std::mutex> lock(mutex_);

  count_--;
  if (count_ == 0) {
    cond_.notify_all();
  }
}

uint32_t CountDownLatch::GetCount() const {
  std::lock_guard<std::mutex> lock(mutex_);

  return count_;
}