#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char** argv)
{
	int fid, novaPri;

	if(argc < 2){
		printf("Modo de uso: %s novaPrioridade\n", argv[0]);
		return 0;
	}

	novaPri = atoi(argv[1]);

	if(fid = fork())
	{
		/*pai*/
		printf("PID do filho: %d\n", fid);
		printf("Nova prioridade passada como argumento: %d\n", novaPri);
		printf("Resultado da syscall: %d\n", setpriority_ep(fid, novaPri));
	}
	else
	{
		/*filho*/
		sleep(60);
	}

	return 0;
}