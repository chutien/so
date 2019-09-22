#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <chrono>
#include <functional>
#include <atomic>
#include "sem.hpp"

#define THREAD_NUM 100

std::atomic<int> crit_counter;
std::atomic<int> op_cnt;

semaphore s(3);

class Thread {
public:
  void run(std::function<void()> func) {
     thread_ = std::thread(func);
  }
  void join() {
     if(thread_.joinable())
        thread_.join();
  }
private:
  std::thread thread_;
};

void threadBody() {
    for(int i=0;i<500;++i) {
      s.down(1);
      ++crit_counter;
      ++op_cnt;
      // If crit_counter>=4 we got at least 4 functions in the critical section (Uuups!)
      assert(crit_counter < 4);
      
      if(op_cnt % 30 == 0) {
        std::cout<<op_cnt.load()<<"/"<<50000<<"\n";
        std::cout.flush();
      }
      
      if(i%10 == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      } else if (i%3 == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
      
      --crit_counter;
      // If crit_counter>=4 we got at least 4 functions in the critical section (Uuups!)
      assert(crit_counter < 4);
      s.up(1);
    }
}

/**
 * This is multithreading test of multiple threads.
 * Just some fun! Yeah!
 *
 */
int main(void) {
  std::ios_base::sync_with_stdio(0);
  
  crit_counter.store(0);
  op_cnt.store(0);
  
  std::cout << "[i] MULTI multithreading test :)\n";
  std::cout.flush();

  Thread threads[THREAD_NUM];
  for(int x=0;x<THREAD_NUM;++x) {
    threads[x] = Thread();
  }
  
  for(int x=0;x<THREAD_NUM;++x) {
    threads[x].run(threadBody);
  }
  
  for(int x=0;x<THREAD_NUM;++x) {
    threads[x].join();
  }
  
  std::cout << "[i] MULTI multithreading test is OK\n";
  std::cout.flush();
  
  return 0;
}
