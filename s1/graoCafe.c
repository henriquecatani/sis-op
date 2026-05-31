// Henrique Bottan Catani 210305
// Othavio Frison Pedretti 207705
// graoCafe.c

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

static int hooper = 0;
static int provadores_disp = 0;
static int tokens_processados = 0;
static int tokens_descartados = 0;

static int T;
static int H;
static int P;

static int min;
static int max;

static int fim = 0;

sem_t mutex_sem; // binario
sem_t sem_tokens; // contador
sem_t sem_vagas; // contador

static void *mestre_torra(void *param)
{
	for ( int i = 1; i < T; i++ )
	{
		usleep((rand() % max - min + 1) + min);

		int result = sem_wait(&sem_vagas);

		if (result == 0)
		{
			sem_wait(&mutex_sem);
			hooper++;
			printf("Token: %d\n", i);
			printf("Provador disponivel\n");
			sem_post(&mutex_sem);

			sem_post(&sem_tokens);
		} 
		else { // hooper cheio
			sem_wait(&mutex_sem);
			tokens_descartados++;
			printf("Token: %d\n", i);
			printf("Hooper cheio, sem provadores disponiveis\n");
			printf("Token descartado\n");
			sem_post(&mutex_sem);
		}
	}
}

static void *provador(void *param)
{
	sem_wait(&mutex_sem);
	printf("Provador %d criado\n", (int) param);
	sem_post(&mutex_sem);

	while (1)
	{
		sem_wait(&sem_tokens); // espera token - se n tiver token, fica bloqueado

		tokens_processados++;
		printf("Provador %d processando token\n", (int) param);
		sem_post(&mutex_sem);

		if (fim) break;
		// conseguiu token 

		sem_wait(&mutex_sem);
		hooper--;
		sem_post(&mutex_sem);

		sem_post(&sem_vagas);

		sem_wait(&mutex_sem);
		printf("Provador %d processando token\n", (int) param);
		sem_post(&mutex_sem);

		usleep((rand() % 41) + 10);
	}

	sem_wait(&mutex_sem);
	printf("Provador %d finalizou\n", (int)param);
	sem_post(&mutex_sem);
}

int main (void) 
{
	// todo: verificar args de input
	// args: T, H, P, min, max

	srand(time(NULL)); // para depois gerar numeros aleatorios

	sem_init(&sem_tokens, 0, 0); // comeca com 0 tokens prontos
	sem_init(&mutex_sem, 0, 1); // comeca desbloqueado
	sem_init(&sem_vagas, 0, H); // comeca com H vagas disponiveis no hooper

	pthread_t t_provadores[P];
	for (int i = 0; i < P; i++)
	{
		pthread_create(&t_provadores[i], NULL, provador, (void *) i);
	}

	pthread_t t_mestre;
	pthread_create(&t_mestre, NULL, mestre_torra, NULL);

	pthread_join(t_mestre, NULL);

	// fim
	fim = 1;

	// acorda todos os provadores bloqueados para que possam encerrar
	for (int i = 0; i < P; i++)
	{
		sem_post(&sem_tokens);
	}

	// aguarda todos os provadores terminarem
	for (int i = 0; i < P; i++)
	{
		pthread_join(t_provadores[i], NULL);
	}

	// relatorio final
	printf("\nRelatorio final:\n");
	printf("Tokens processados: %d\n", tokens_processados);
	printf("Tokens descartados: %d\n", tokens_descartados);
	printf("Hooper (ocupados): %d\n", hooper);

	sem_destroy(&mutex_sem);	
	sem_destroy(&sem_tokens);
	sem_destroy(&sem_vagas);

	return 0;
}


