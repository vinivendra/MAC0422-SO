#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char** argv)
{

	int pidPai;

	pidPai = getpid();

	if(fork())
	{
		/*pai*/
		printf("PID do pai: %d\n", pidPai);
		sleep(60);
	}
	else
	{
		/*filho*/
		printf("Filho vai tentar mudar a prioridade do pai\n");
		printf("Resultado da syscall: %d\n", setpriority_ep(pidPai, 1));
	}

	return 0;
}