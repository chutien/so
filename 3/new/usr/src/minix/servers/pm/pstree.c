#include <stdio.h>
#include "pm.h"
#include "mproc.h"

/* Array stores relevant indices to mproc sorted by parent's realpid and realpid,
   where relevant is when it meets conditions of is_child_ok. */
static size_t mproc_index[NR_PROCS];

static int is_child_ok(pid_t pid, int uid, int gid, size_t i) {
  return (mproc[i].mp_flags & IN_USE)
    && (mproc[mproc[i].mp_parent].mp_pid == pid)
    && (mproc[i].mp_realuid == uid)
    && (mproc[i].mp_realgid == gid)
    && (mproc[mproc[i].mp_parent].mp_pid != mproc[i].mp_pid);
}

/* Returns number of children meeting conditions for pid, uid and gid
   and puts indices of relevant children in order beginning at
   the mproc_index[idx]. Insertion sort is used since mp_proc array
   stores processes more or less in increasing order of pids. */
static size_t insort_children(pid_t pid, int uid, int gid, size_t idx)
{
  size_t i, j;
  size_t count = 0;
  for (i = 0; i < NR_PROCS; ++i) {
    if (!is_child_ok(pid, uid, gid, i))
      continue;
    for (j = idx + count++; j > idx && mproc[mproc_index[j - 1]].mp_pid > mproc[i].mp_pid; --j)
      mproc_index[j] = mproc_index[j - 1];
    mproc_index[j] = i;
  }
  return count;
}

static void do_pstree_children(pid_t pid, int uid, int gid, int idx, int generation)
{
  size_t i, j;
  size_t count = insort_children(pid, uid, gid, idx);
  pid_t child_pid;
  for (i = idx; i < idx+count; ++i) {
    for (j = 0; j < generation; ++j)
      printf("---");
    child_pid = mproc[mproc_index[i]].mp_pid;
    printf("%d\n", child_pid);
    do_pstree_children(child_pid, uid, gid, idx + count, generation + 1);
  }
  return;
}

int do_pstree(void)
{
  pid_t pid = m_in.m_pm_pstree.pid;
  int uid = m_in.m_pm_pstree.uid;
  gid_t gid = mproc[_ENDPOINT_P(m_in.m_source)].mp_realgid;
  int i;
  for (i = 0; i < NR_PROCS; ++i) {
    if (mproc[i].mp_pid == pid
        && (mproc[i].mp_flags & IN_USE)
        && (mproc[i].mp_realuid == uid)
        && (mproc[i].mp_realgid == gid)) {
      printf("%d\n", pid);
      do_pstree_children(pid, uid, gid, 0, 1);
      break;
    }
  }
  return OK;
}
