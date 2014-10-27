/* ################################################## */

#include <lib.h>
#include <unistd.h>

PUBLIC int setpriority_ep2(procid, procpri)
int procid;
int procpri;
{
    message m;
    /*Popula a mensagem com os argumentos*/
    
    m.m1_i1 = procid;
    m.m1_i2 = procpri;
    
    return(_syscall(MM, SETPRIORITYEP2, &m));
}

/* ################################################ */
