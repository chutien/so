#include <iostream>
#include <assert.h>
#include "sem.hpp"

/**
 * Simple semaphore test.
 * Basic simple up/downs
 *
 */
int main(void) {
  std::ios_base::sync_with_stdio(0);
  
  std::cout << "[i] Hello :)\n";

  semaphore s(0);
  std::cout << "[i] s = " << s << "\n";
  std::cout.flush();
  
  s.up(3);
  std::cout << "[i] s = " << s << "\n";
  std::cout.flush();
  
  s.down(2);
  std::cout << "[i] s = " << s << "\n";
  std::cout.flush();
  
  s.up(1);
  std::cout << "[i] s = " << s << "\n";
  std::cout.flush();
  
  s.down(2);
  std::cout << "[i] s = " << s << "\n";
  std::cout.flush();
  
  for(int i=0;i<2137;i++) {
    s.up(0);
    s.down(0);
    s.down(0);
    s.up(1);
    s.down(0);
    s.down(1);
    assert(s.get() == 0);
  }
  
  assert(s.get() == 0);

  semaphore zeros(0);
  for(int r=0;r<1000;++r) {
    zeros.up(0);
    zeros.down(0);
    zeros.up(0);
  }
  
  semaphore pretty_max(2147483647);
  pretty_max.down(2147483646);
  pretty_max.down(1);
  pretty_max.up(2147483647);
  
  assert(pretty_max.get() == 2147483647);
  
  pretty_max.down(2147483646);
  
  assert(pretty_max.get() == 1);
  
  
  return 0;
}
