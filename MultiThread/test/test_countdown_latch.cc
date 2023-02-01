#include <iostream>
#include <thread>
#include <vector>

#include "countdown_latch.h"

int main() {
  std::vector<std::thread> v(10);
  CountDownLatch latch(10);

  auto thread_func = [&latch]() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    latch.CountDown();
  };

  for (int i = 0; i < 10; i++) {
    v[i] = std::thread(thread_func);
  }

  latch.Await(1);

  for (auto& t : v) {
    t.join();
  }

  return 0;
}