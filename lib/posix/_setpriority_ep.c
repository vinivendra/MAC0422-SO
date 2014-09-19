/* ################################################## */

#include <lib.h>
#include <unistd.h>

PUBLIC int setpriority_ep(procid, procpri)
int procid;
int procpri;
{
	message m;

  return(_syscall(MM, SETPRIORITYEP, &m));
}

/* ################################################ */
