#include <lib.h>
#include <unistd.h>

int setsjf(int expected_time) /* sjf_2018 */
{
  message m;
  m.m1_i1 = expected_time;
  return _syscall(PM_PROC_NR, PM_SETSJF, &m);
}
