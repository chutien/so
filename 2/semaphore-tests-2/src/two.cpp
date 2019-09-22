#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <chrono>
#include "sem.hpp"

/**
 * Drop-in replacement for binary semaphore
 * C++0x quick and dirty implementation
 *
 */
class cxx_binary_semaphore {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
public:
    cxx_binary_semaphore(int count_ = 0) : count(count_) {}

    inline void up(int trash) {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        cv.notify_one();
    }

    inline void down(int trash) {
        std::unique_lock<std::mutex> lock(mtx);

        while(count == 0){
            cv.wait(lock);
        }
        count--;
    }
};


volatile int crit_counter = 0;

semaphore s(1);

void f() {
    for(int i=0;i<100;++i) {
      assert(s.down_time(1) > 0);
      ++crit_counter;
      // If crit_counter!=1 we got two functions in the critical section (Uuups!)
      assert(crit_counter == 1);
      
      std::this_thread::sleep_for(std::chrono::milliseconds((i%3)*10+10));
      
      --crit_counter;
      // If crit_counter!=0 we got two functions in the critical section (Uuups!)
      assert(crit_counter == 0);
      s.up(1);
    }
}

void g() {
    for(int i=0;i<100;++i) {
      assert(s.down_time(1) > 0);
      ++crit_counter;
      // If crit_counter!=1 we got two functions in the critical section (Uuups!)
      assert(crit_counter == 1);
      
      std::this_thread::sleep_for(std::chrono::milliseconds(((i*i+1)%3)*5+3));

      --crit_counter;
      // If crit_counter!=0 we got two functions in the critical section (Uuups!)
      assert(crit_counter == 0);
      s.up(1);
    }
}

/**
 * This is multithreading test of two threads.
 * Basic stuff basically XD
 *
 */
int main(void) {
  std::ios_base::sync_with_stdio(0);
  setup_logic_time(TIME_MODE_STDLOG);
  
  std::cout << "[i] TWO multithreading test :)\n";
  std::cout.flush();

  std::thread t1{f};
  std::thread t2{g};
  
  t1.join();
  t2.join();
  
  return 0;
}
