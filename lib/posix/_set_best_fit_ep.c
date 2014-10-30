/* ################################################## */

#include <lib.h>
#include <unistd.h>

PUBLIC int set_best_fit_ep(should_use_best_fit)
int should_use_best_fit;
{
    message m;
    /*Popula a mensagem com os argumentos*/
    
    m.m1_i1 = should_use_best_fit;

    return(_syscall(MM, SETBESTFITEP, &m));
}

/* ################################################ */
