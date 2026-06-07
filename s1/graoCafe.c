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
static int provadores_ocupados = 0;
static int tokens_processados = 0;
static int tokens_descartados = 0;
static int id_token = 1;

static int T;
static int H;
static int P;
static int min;
static int max;


sem_t mutex_sem;  // binario
sem_t sem_tokens; // contador
sem_t sem_vagas;  // contador

static void *mestre_torra(void *param)
{
	for (int i = 1; i <= T; i++)
	{
		usleep(((rand() % (max - min + 1)) + min) * 1000);
		sem_wait(&mutex_sem);

		if ( provadores_ocupados < P && !hooper ) {
			printf("Token: %d\n", i);
			printf("Status: Provador disponivel\n");
			provadores_ocupados++;
			printf("Hooper: %d/%d vagas ocupadas\n", hooper, H);

			sem_post(&mutex_sem);
			sem_post(&sem_tokens);
			continue;
		}

		int result = sem_trywait(&sem_vagas);

		if (result == 0)
		{
			hooper++;
			printf("Token: %d\n", i);
			printf("Status: Armazenado no hooper\n");
			printf("Hooper: %d/%d vagas ocupadas\n", hooper, H);

			sem_post(&mutex_sem);
			sem_post(&sem_tokens);
		}
		else
		{ // hooper cheio e sem provadores disponiveis
			tokens_descartados++;
			printf("Token: %d\n", i);
			printf("Token descartado\n");
			printf("Hooper cheio (H=%d), todos os %d provadores ocupados\n", H, P);
			sem_post(&mutex_sem);
		}
	}
	return NULL;
}

static void *provador(void *param)
{
	sem_wait(&mutex_sem);
	int id = (int)(long)param;
	printf("Provador %d criado\n", id);
	sem_post(&mutex_sem);

	while (1)
	{
		sem_wait(&sem_tokens); // espera token - se n tiver token, fica bloqueado

		sem_wait(&mutex_sem);

		if ((tokens_processados + tokens_descartados) >= T) {
			sem_post(&mutex_sem);
			break;
		}

		if (hooper > 0) {
			hooper--;
			sem_post(&sem_vagas); // libera vaga no hooper
			provadores_ocupados++;
		}

		int token_id = id_token++;
		printf("Provador %d processando token %d\n", id, token_id);

		sem_post(&mutex_sem);

		usleep(((rand() % 41) + 10) * 1000); // "processamento" do token (10-50ms)

		sem_wait(&mutex_sem);
		printf("Provador %d finalizou token %d\n", id, token_id);
		provadores_ocupados--;
		tokens_processados++;
		sem_post(&mutex_sem);
	}

	sem_wait(&mutex_sem);
	printf("Provador %d finalizado\n", id);
	sem_post(&mutex_sem);
	return NULL;
}

int main(int argc, char *argv[])
{
	// verificar args de input
	// args: T, H, P, min, max
	if (argc != 6)
	{
		fprintf(stderr, "Uso: %s <P> <H> <T> <min> <max>\n", argv[0]);
		return 1;
	}

	P = atoi(argv[1]);
	H = atoi(argv[2]);
	T = atoi(argv[3]);
	min = atoi(argv[4]);
	max = atoi(argv[5]);

	if (T <= 1 || T >= 100 || H <= 0 || H >= 10 || P <= 0 || P >= 5 
		|| min <= 5 || min >= 10 || max <= min || max >= 50) {
		fprintf(stderr, "Erro: argumentos invalidos\n");
		return 1;
	}

	srand(time(NULL)); // para depois gerar numeros aleatorios

	sem_init(&sem_tokens, 0, 0); // comeca com 0 tokens prontos
	sem_init(&mutex_sem, 0, 1);	 // comeca desbloqueado
	sem_init(&sem_vagas, 0, H);	 // comeca com H vagas disponiveis no hooper

	pthread_t t_provadores[P];
	for (int i = 0; i < P; i++)
		pthread_create(&t_provadores[i], NULL, provador, (void *)(long)(i + 1));

	pthread_t t_mestre;
	pthread_create(&t_mestre, NULL, mestre_torra, NULL);

	pthread_join(t_mestre, NULL);

	while (1)
	{
		sem_wait(&mutex_sem);
		int total = tokens_processados + tokens_descartados;
		sem_post(&mutex_sem);

		if (total >= T) break;

		usleep(1000); // espera um pouco antes de verificar novamente
	}

	// acorda todos os provadores bloqueados para que possam encerrar
	for (int i = 0; i < P; i++)
		sem_post(&sem_tokens);

	// aguarda todos os provadores terminarem
	for (int i = 0; i < P; i++)
		pthread_join(t_provadores[i], NULL);

	printf("\nRelatorio final:\n");
	printf("Tokens processados: %d\n", tokens_processados);
	printf("Tokens descartados: %d\n", tokens_descartados);
	printf("Tokens no Hooper: %d\n", hooper);
	printf("Total de tokens: %d\n", tokens_processados + tokens_descartados);

	sem_destroy(&mutex_sem);
	sem_destroy(&sem_tokens);
	sem_destroy(&sem_vagas);

	return 0;
}