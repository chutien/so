#include <iostream>

#define TIME_MODE_STDLOG    101  // Change mode to STDLOG and leave value unchanged
#define TIME_MODE_TINVINCR  102  // Change mode to TINVINCR and leave value unchanged
#define TIME_MODE_ZERO      103  // Time is always zero XD

extern "C" {
  
  void proberen(int32_t *semaphore, int32_t value);
  void verhogen(int32_t *semaphore, int32_t value);
  uint64_t proberen_time(int32_t *semaphore, int32_t value);
  
}

int next_logic_time();
int get_logic_time();
int setup_logic_time(const int mode);

class semaphore {
private:
  int32_t* sem;
  
public:

  semaphore() = delete;
  
  explicit semaphore(const int value) {
    sem = (int32_t*) malloc(sizeof(int32_t));
    *sem = value;
  }
  
  semaphore(const semaphore& s) {
    sem = s.sem;
  }
  
  void down(const int value) {
    proberen(sem, value);
  }
  
  void up(const int value) {
    verhogen(sem, value);
  }
  
  long long down_time(const int value) {
    return (long long) proberen_time(sem, value);
  }
  
  int get() const {
    return *sem;
  }
  
  bool isZero() const {
    return *sem == 0;
  }
};

std::ostream& operator <<(std::ostream& stream, const semaphore& s) {
  return stream << s.get();
}