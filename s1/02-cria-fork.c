#include <stdio.h>
#include <unistd.h>

int main (void)
{
	fork();
	printf("Hello World\n");
	fflush(stdout);
	return 0;
}

