#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void)
{
	int x = 1;
	pid_t pid;

	printf("mem fork 1\n");

	pid = fork();

	if (pid < 0)
	{
		perror("fork");
		exit(1);
	}

	if (pid == 0) 
	{
		printf("Processo filho criado com PID: %d\n", getpid());
		x++;
		printf("Filho tem x = %d\nEndereço do x no filho: %p\n", x, (void*)&x);
	} else {
		printf("Processo pai dorme por 1s\n");
		sleep(1);
		x--;
		printf("Processo pai continua com PID: %d\n", getpid());
		printf("Pai tem x = %d\nEndereço do x no pai: %p\n", x, (void*)&x);
	}

	return 0;
}

