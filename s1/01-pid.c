#include <stdio.h>
#include <unistd.h>

int main (void)
{
	pid_t pid_processo = getpid();
	pid_t pid_pai = getppid();

	printf("PID: %d\n", pid_processo);
	printf("PID: %d\n", pid_pai);

	return 0;
}
