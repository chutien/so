#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <chrono>
#include <functional>
#include <atomic>
#include "sem.hpp"

#define THREAD_NUM 200

std::atomic<int> crit_counter;
std::atomic<int> op_cnt;
semaphore s(20);

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

std::mutex mut;

void threadBody() {
    for(int i=1;i<20;++i) {
      s.down(i);
      
      {
        std::lock_guard<std::mutex> lock(mut);
        
        ++crit_counter;
        ++op_cnt;
        
        const int v = crit_counter.load();
        const int opv = op_cnt.load();
        if(v >= 21) {
          std::cout<<"[!] TOO MANY PROCESSES IN CRIT. SEC.\n";
          std::cout<<"Semaphore s   = "<<s<<"\n";
          std::cout<<"Crit-counter  = "<<v<<"\n";
          std::cout<<"Op-counter    = "<<opv<<"\n";
          std::cout.flush();
          assert(v <= 20);
        }
      }
      
      if(op_cnt % 50 == 0) {
        std::cout<<op_cnt.load()<<"/"<<3800<<"\n";
        std::cout.flush();
      }
      
      if (i%2 == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
      
      {
        std::lock_guard<std::mutex> lock(mut);
        
        --crit_counter;
        
        const int v = crit_counter.load();
        const int opv = op_cnt.load();
        if(v >= 21) {
          std::cout<<"[!] TOO MANY PROCESSES IN CRIT. SEC.\n";
          std::cout<<"Semaphore s   = "<<s<<"\n";
          std::cout<<"Crit-counter  = "<<v<<"\n";
          std::cout<<"Op-counter    = "<<opv<<"\n";
          std::cout.flush();
          assert(v <= 20);
        }
      }
      
      s.up(i);
    }
}

/**
 * This is multithreading test of REALY MANY THREADS LOL XD
 *
 */
int main(void) {
  std::ios_base::sync_with_stdio(0);
  
  crit_counter.store(0);
  op_cnt.store(0);
  
  std::cout << "[i] DAFUQ multithreading test :)\n";
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
  
  std::cout << "[i] DAFUQ multithreading test is OK\n";
  std::cout.flush();
  
  return 0;
}
