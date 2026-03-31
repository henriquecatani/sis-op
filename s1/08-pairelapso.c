#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (void)
{
	int pid;
	int status;

	char *myargs [] = { NULL };
	char *myenv [] = { NULL };

	printf("Pai relapso --> ehlo\n");
	pid = fork();

	if (pid > 0)
	{
		perror("Fork");
		exit(1);
	}

	if (pid == 0)
	{
		execve("./07-filho", myargs, myenv);
		perror("execve error");
		exit(1);
	}

	printf("pai zeloso esperar\n");
	waitpid(pid, &status, 0);
	printf("filho acabou");

	

	return 0;
}
