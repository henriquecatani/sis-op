#include <stdio.h>
#include <unistd.h>

int main (void)
{
	fork();
	fork();
	fork();



	printf("Hello World\n");
	fflush(stdout);

	// printa 8 hello wordl
	return 0;
}
