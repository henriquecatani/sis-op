#include <stdio.h>
#include <unistd.h>

int main (void)
{
	char input[20];

	while (input != "exit") {
		scanf("%19s", input);
		char *progname = input;
		char *argv = { progname, NULL };
		char *envp = { NULL };

		pid_t res_fork = fork();

		if (res_fork == 0)
		{
			execve(progname, envp);
			exit(0);
		}

	}
	return 0;
}
