#include <lib.h>
#include <unistd.h>

int pstree(pid_t pid, int uid) {
  message m;
  m.m_pm_pstree.pid = pid;
  m.m_pm_pstree.uid = uid;
  return _syscall(PM_PROC_NR, PM_PSTREE, &m);
}
