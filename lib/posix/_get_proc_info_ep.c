/* ################################################## */

#include <lib.h>
#include <unistd.h>

PUBLIC int get_proc_info_ep(nome_do_role)
int nome_do_role;
{
    message m;
    /*Popula a mensagem com os argumentos*/
    
    m.m1_i1 = nome_do_role;
    
    return(_syscall(MM, GETPROCINFOEP, &m));
}

/* ################################################ */
