#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void)
{
	char *progname = "ls";

	char *argv[] = { NULL };
	char *envp[] = { NULL };

	printf("Executa\n");

	execve(progname, argv, envp);
	perror("execve falhou");
	return 1;
}
