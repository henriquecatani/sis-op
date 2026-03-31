#include <stdio.h>
#include <unistd.h>

int main (void)
{
	pid_t pid_processo = getpid();
	printf("PID: %d\n", pid_processo);
	
	pid_t resultado_fork = fork();

	if (resultado_fork == -1)
	{
		perror("Erro ao criar processo filho");
		return 1;
	} else if (resultado_fork == 0) {
		printf("PID filho: %d\n", getpid());	
	}

	printf("Codigo compartilhado\n");
	fflush(stdout);
	return 0;
}
