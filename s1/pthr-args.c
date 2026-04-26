// henriquecatani
// cada thread imprime uma string e seu numero

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_estado(void *num) 
{
    printf("Rio Grande do Sul\n");
    int *numero = (int *) num;
    printf("Thread %d\n", *numero);
    return NULL;
}

void *thread_terra(void *num) 
{
    printf("Terra de Gauchos e da UPF\n");
    int *numero = (int *) num;
    printf("Thread %d\n", *numero);
    return NULL;
}

int main (int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        perror("Uso: [progname] <number_of_threads>\n");
        return 1;
    }

    int N = atoi(argv[1]);
    pthread_t threads[N];
    int valores[N];

    for (int i = 0; i < N; i++) 
    {
        valores[i] = i;  // pro valor nao mudar
        if (i < N / 2) 
        {
            pthread_create(&threads[i], NULL, thread_estado, &valores[i]);
        } else 
        {
            pthread_create(&threads[i], NULL, thread_terra, &valores[i]);
        }
    }

    for (int i = 0; i < N; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
