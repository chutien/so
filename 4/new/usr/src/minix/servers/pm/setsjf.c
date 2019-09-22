#include <stdio.h>
#include <errno.h>
#include "pm.h"
#include "mproc.h"

int do_setsjf(void) /* sjf_2018 */
{
  int expected_time = m_in.m1_i1;
  if (expected_time < 0 || expected_time > MAX_SJFPRIO) {
    errno = EINVAL;
    return -1;
  }

  printf("Hello World!");
  return 0;
}
