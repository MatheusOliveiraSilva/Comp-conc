/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 5 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5

/* Variaveis globais */
int chegadas = 0;
int sentados = 0;
pthread_mutex_t x_mutex;
pthread_cond_t chegada_cond;
pthread_cond_t sentado_cond;

void *chegada(void *arg) {
    int thread_id = *(int*)arg;  // obtem o ID personalizado da thread

    if (thread_id == 0) {
        printf("Oi José!\n");
    }
    if (thread_id == 1) {
        printf("Oi Maria!\n");
    }

    // faz o lock porque vai alternar a variavel global e checa se ela for 2, o que significa 
    // que as duas pessoas já chegaram, e nesse caso sinaliza a variavel de condição para mandarem
    // sentar ( funcao da thread 3 ).
    pthread_mutex_lock(&x_mutex);
    chegadas++;
    if (chegadas == 2) {
        pthread_cond_signal(&chegada_cond);
    }
    pthread_mutex_unlock(&x_mutex);

    pthread_exit(NULL);
}

void *sentem(void *arg) {

    pthread_mutex_lock(&x_mutex);

    // está escrito literalmente "enquanto não for 2, espere"
    while (chegadas != 2) {pthread_cond_wait(&chegada_cond, &x_mutex);}

    // depois que sair do while de cima, significa que as duas pessoas chegaram, entao manda sentarem.
    printf("Sentem-se por favor.\n");
    sentados++;
    pthread_cond_broadcast(&sentado_cond); // libera os waits da thread 4 e 5. precisa ser broadcast.

    pthread_mutex_unlock(&x_mutex);
    
    pthread_exit(NULL);
}

void *saida(void *arg) {

    
    pthread_mutex_lock(&x_mutex);

    // está escrito literalmente "enquanto não for 1, espere".
    while(sentados != 1) {pthread_cond_wait(&sentado_cond, &x_mutex);}

    pthread_mutex_unlock(&x_mutex);

    int thread_id = *(int*)arg;  // obtem o ID personalizado da thread

    if (thread_id == 3) {
        printf("Tchau José!\n");
    }
    if (thread_id == 4) {
        printf("Tchau Maria!\n");
    }

    pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e as variaveis de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&chegada_cond, NULL);
  pthread_cond_init (&sentado_cond, NULL);

  int thread_ids[NTHREADS] = {0, 1, 2, 3, 4};

  // cria as threads.
  pthread_create(&threads[0], NULL, chegada, &thread_ids[0]);
  pthread_create(&threads[1], NULL, chegada, &thread_ids[1]);
  pthread_create(&threads[2], NULL, sentem, &thread_ids[2]);
  pthread_create(&threads[3], NULL, saida, &thread_ids[3]);
  pthread_create(&threads[4], NULL, saida, &thread_ids[4]);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\n");
  
  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&chegada_cond);
  pthread_cond_destroy(&sentado_cond);

  return 0;
}