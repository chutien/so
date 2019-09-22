#include <iostream>
#include <assert.h>
#include "sem.hpp"

;

/**
 * Testuje proberen_time z zegarem logicznym.
 * (każde wywołanie +1 do czasu w trybie LOGIC_MODE)
 *
 */
int main(void) {
  std::ios_base::sync_with_stdio(0);
  
  std::cout << "[i] Basic down_time test...\n";
  std::cout.flush();
  
  semaphore s(5);
  // Time is 1, 2, 3, 4, 5, 6...
  setup_logic_time(TIME_MODE_STDLOG);
  
  assert( s.down_time(2) == 1);
  s.up(2);
  assert( s.down_time(2) == 1);
  s.up(3);
  assert( s.down_time(2) == 1);
  
  //Time is 1, 3, 6, 10, 15, ...
  setup_logic_time(TIME_MODE_TINVINCR);
  s.up(3);
  assert( s.down_time(2) == 2);
  s.up(3);
  assert( s.down_time(2) == 4);
  s.up(4);
  assert( s.down_time(2) == 6);
  
  // Time is 1, 2, 3, 4, 5, 6...
  setup_logic_time(TIME_MODE_STDLOG);
  for(int i=0;i<100;++i) {
    s.up(i+2);
    assert( s.down_time(i) > 0 );
  }
  
  std::cout << "[i] Basic down_time test OK\n";
  std::cout.flush();
  
  return 0;
}