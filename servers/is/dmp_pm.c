/* This file contains procedures to dump to PM' data structures.
 *
 * The entry points into this file are
 *   mproc_dmp:   	display PM process table	  
 *
 * Created:
 *   May 11, 2005:	by Jorrit N. Herder
 */

#include "is.h"
#include "../pm/mproc.h"
#include <timers.h> 

PUBLIC struct mproc mproc[NR_PROCS];

/*===========================================================================*
 *				mproc_dmp				     *
 *===========================================================================*/
PRIVATE char *flags_str(int flags)
{
	static char str[10];
	str[0] = (flags & WAITING) ? 'W' : '-';
	str[1] = (flags & ZOMBIE)  ? 'Z' : '-';
	str[2] = (flags & PAUSED)  ? 'P' : '-';
	str[3] = (flags & ALARM_ON)  ? 'A' : '-';
	str[4] = (flags & TRACED)  ? 'T' : '-';
	str[5] = (flags & STOPPED)  ? 'S' : '-';
	str[6] = (flags & SIGSUSPENDED)  ? 'U' : '-';
	str[7] = (flags & REPLY)  ? 'R' : '-';
	str[8] = (flags & ONSWAP)  ? 'O' : '-';
	str[9] = (flags & SWAPIN)  ? 'I' : '-';
	str[10] = (flags & DONT_SWAP)  ? 'D' : '-';
	str[11] = (flags & PRIV_PROC)  ? 'P' : '-';
	str[12] = '\0';

	return str;
}

/* ######################################## */

PUBLIC void memorymap_ep()
{
  struct mproc *mp;
  int i, j,mini, n=0;
  static phys_clicks usedClicks = 0, totalClicks = 0;
  static int prev_i = 0, prev_j = 0, tam = 0;
  static struct mproc **vetor = NULL;

  printf("Process manager (PM) memory dump\n");

  if(!vetor)
    {
      tam = NR_PROCS;
      vetor = malloc(tam*sizeof(struct mproc*));
      for(i = 0; i<NR_PROCS; i++)
          vetor[i] = &mproc[i];
      for(i = 0; i < tam; i++)
      {
        mini = i;
        for(j = i; j < tam; j++)
          if(vetor[j]->mp_seg[D].mem_phys < vetor[mini]->mp_seg[D].mem_phys)
            mini = j;
        mp = vetor[i];
        vetor[i] = vetor[mini];
        vetor[mini] = mp;
      }
    }



  getsysinfo(PM_PROC_NR, SI_PROC_TAB, mproc);
  if(!totalClicks)
    getsysinfo(PM_PROC_NR, SI_USU_MEM, &totalClicks);

  printf("-pid-\t-p_ini-\t-p_end-\n");
  for (i=prev_i; i<tam; i++) {
    mp = vetor[i];
    if (mp->mp_pid == 0 && i != PM_PROC_NR) continue;
    if (n > 20) break;
    

    if(mp->mp_flags & SEPARATE)
    {
      usedClicks += mp->mp_seg[T].mem_len;
      usedClicks +=  mp->mp_seg[S].mem_phys+mp->mp_seg[S].mem_len -
                        mp->mp_seg[D].mem_phys;
      if(mp->mp_seg[T].mem_phys+mp->mp_seg[T].mem_len == 
          mp->mp_seg[D].mem_phys)
      {
        n++;
        /*Texto e data seguidos*/
        printf("%d\t%u\t%u\n",mp->mp_pid, mp->mp_seg[T].mem_phys,  
          mp->mp_seg[S].mem_phys+mp->mp_seg[S].mem_len);
      }
      else
      { n+=2;
        printf("%d\t%u\t%u\n",mp->mp_pid , mp->mp_seg[T].mem_phys,
        mp->mp_seg[T].mem_phys+mp->mp_seg[T].mem_len);        
        printf("%d\t%u\t%u\n",mp->mp_pid , mp->mp_seg[D].mem_phys,
        mp->mp_seg[S].mem_phys+mp->mp_seg[S].mem_len);
        

      }
    }
    else
    {
      n++;
      printf("%d\t%u\t%u\n",mp->mp_pid , mp->mp_seg[D].mem_phys,
        mp->mp_seg[S].mem_phys+mp->mp_seg[S].mem_len);
      usedClicks +=  mp->mp_seg[S].mem_phys+mp->mp_seg[S].mem_len -
                        mp->mp_seg[D].mem_phys;        
    }

    


    

      
  }
  if (i >= NR_PROCS)
  {
    printf("Memoria livre: %u\n || Memoria usada: %u\n", totalClicks, usedClicks);
    i = 0;
    usedClicks = totalClicks = 0;
    free(vetor);
    vetor = NULL;
    tam = 0;
  }
  else printf("--more--\r");
  prev_i = i;
  prev_j = j;
}


/* ####################################### */

PUBLIC void mproc_dmp()
{
  struct mproc *mp;
  int i, n=0;
  static int prev_i = 0;

  printf("Process manager (PM) process table dump\n");

  getsysinfo(PM_PROC_NR, SI_PROC_TAB, mproc);

  printf("-process- -nr-prnt- -pid/ppid/grp- -uid--gid- -nice- -flags------\n");
  for (i=prev_i; i<NR_PROCS; i++) {
  	mp = &mproc[i];
  	if (mp->mp_pid == 0 && i != PM_PROC_NR) continue;
  	if (++n > 22) break;
  	printf("%8.8s %4d%4d  %4d%4d%4d    ", 
  		mp->mp_name, i, mp->mp_parent, mp->mp_pid, mproc[mp->mp_parent].mp_pid, mp->mp_procgrp);
  	printf("%d(%d)  %d(%d)  ",
  		mp->mp_realuid, mp->mp_effuid, mp->mp_realgid, mp->mp_effgid);
  	printf(" %3d  %s  ", 
  		mp->mp_nice, flags_str(mp->mp_flags)); 
  	printf("\n");
  }
  if (i >= NR_PROCS) i = 0;
  else printf("--more--\r");
  prev_i = i;
}

/*===========================================================================*
 *				sigaction_dmp				     *
 *===========================================================================*/
PUBLIC void sigaction_dmp()
{
  struct mproc *mp;
  int i, n=0;
  static int prev_i = 0;
  clock_t uptime;

  printf("Process manager (PM) signal action dump\n");

  getsysinfo(PM_PROC_NR, SI_PROC_TAB, mproc);
  getuptime(&uptime);

  printf("-process- -nr- --ignore- --catch- --block- -tomess-- -pending-- -alarm---\n");
  for (i=prev_i; i<NR_PROCS; i++) {
  	mp = &mproc[i];
  	if (mp->mp_pid == 0 && i != PM_PROC_NR) continue;
  	if (++n > 22) break;
  	printf("%8.8s  %3d  ", mp->mp_name, i);
  	printf(" 0x%06x 0x%06x 0x%06x 0x%06x   ", 
  		mp->mp_ignore, mp->mp_catch, mp->mp_sigmask, mp->mp_sig2mess); 
  	printf("0x%06x  ", mp->mp_sigpending);
  	if (mp->mp_flags & ALARM_ON) printf("%8u", mp->mp_timer.tmr_exp_time-uptime);
  	else printf("       -");
  	printf("\n");
  }
  if (i >= NR_PROCS) i = 0;
  else printf("--more--\r");
  prev_i = i;
}

