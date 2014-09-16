/*################################################*/
//
//  ep.h
//  Minix
//
//  Created by Vinícius Vendramini on 16/9/14.
//
//

#include <lib.h>

PUBLIC void ep(int eppid, int eppriority)
{
    message m;
    
    _syscall(MM, SETPRIORITYEP, &m);
    
    return 0;
}
/*################################################*/
