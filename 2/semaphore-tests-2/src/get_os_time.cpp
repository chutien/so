#include<stdint.h>

#define MODE_READ      0    // Leave mode unchanged just read value
#define MODE_NEXT      1    // Leave mode unchanged, move to the next value and return it
#define MODE_RESET     2    // Reset time to 0 leaving mode unchanged
#define MODE_STDLOG    101  // Change mode to STDLOG and leave value unchanged
#define MODE_TINVINCR  102  // Change mode to TINVINCR and leave value unchanged
#define MODE_ZERO      103  // Time is always zero XD

// Helper function :>
int __set_logic_timer_mode__(const int mode) {
  static int counter = 0;
  static int current_mode = MODE_STDLOG;
  static int intv = 1;
  
  if(mode == MODE_READ) {
    return counter;
  } else if(mode == MODE_NEXT) {
    if(current_mode == MODE_STDLOG) {
      ++counter;
    } else if(current_mode == MODE_TINVINCR) {
      counter += intv;
      ++intv;
    } else if(current_mode == MODE_ZERO) {
      counter = 0;
    } else {
      counter = 0;
    }
    return counter;
  } else if(mode == MODE_RESET) {
    counter = 0;
    intv = 1;
  } else {
    current_mode = mode;
  }
  
  return counter;
}

int next_logic_time() {
  return __set_logic_timer_mode__(MODE_NEXT);
}

int get_logic_time() {
  return __set_logic_timer_mode__(MODE_READ);
}

int setup_logic_time(const int mode) {
  __set_logic_timer_mode__(mode);
  __set_logic_timer_mode__(MODE_RESET);
  return get_logic_time();
}

extern "C" {

  uint64_t get_os_time(void) {
    return next_logic_time();
  }

}
