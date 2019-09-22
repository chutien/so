#include <iostream>
#include <assert.h>
#include "sem.hpp"

/**
 * This test checks if we hang on the samaphore.
 * It should never terminate
 */
int main(void) {
  std::ios_base::sync_with_stdio(0);
  
  std::cout << "[i] Hello :)\n";

  semaphore s(69);
  s.up(1);
  
  // Hang here...
  s.down(71);

  return 0;
}
