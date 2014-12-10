/* ################################################## */

#include <lib.h>
#include <unistd.h>
#include <string.h>

PUBLIC int lsr(file_name)
char *file_name;
{
    message m;
    /*Popula a mensagem com os argumentos*/
    
    m.m1_p1 = file_name;
    m.m1_i1 = strlen(file_name) + 1;
    
    return(_syscall(FS, LSR, &m));
}

/* ################################################ */
