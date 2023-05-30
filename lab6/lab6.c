#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N        10
#define PTHREADS 2
#define CTHREADS 4
#define NTHREADS PTHREADS+CTHREADS

// Variaveis globais
sem_t slotCheio;
sem_t slotVazio;
sem_t mutexProdutor;
sem_t mutexConsumidor;
int buffer[N];

void insere(int tid) {
    // mensagem de log para testes futuros mostrando que chegou produtora com intenção de inserir.
    printf("Produtora[%d] chegou e quer inserir.\n", tid);
    sem_wait(&slotVazio);
    
    // depois que passamos do wait, mais uma mensagem de log para registrar quando a produtora conseguiu inserir.   
    printf("Chegou a vez da produtora[%d] que achou posicao vazia no buffer.\n", tid);
    sem_wait(&mutexProdutor);

    // insere dados no buffer, usando seu id como identidade dessa inserção.
    for (int i = 0; i < N; i++) {
      buffer[i] = tid*i;
    }

    // mensagem de log para testes futuros mostrando que a produtora conseguiu inserir.
    printf("Produtora[%d] finalizou a inserção dos itens no buffer.\n", tid);
    sem_post(&mutexProdutor);
    
    for (int i = 0; i < N; i++) sem_post(&slotCheio);
}

int retira(int tid) {
    int item;
    static int out = 0;

    // mensagem de log para testes futuros mostrando que chegou consumidora com intenção de retirar.
    printf("Consumidora[%d] chegou e quer retirar.\n", tid);
    sem_wait(&slotCheio);

    // depois que passamos do wait, mais uma mensagem de log para registrar quando a consumidora conseguiu retirar.
    printf("A thread consumidora[%d] achou posicao cheia no buffer e vai entrar na lista de espera.\n", tid);
    sem_wait(&mutexConsumidor);

    item = buffer[out];

    // retira dados do buffer 
    printf("Chegou a vez da thread consumidora[%d] e ela conseguiu retirar %d da posicao %d.\n", tid, item, out);
    out = (out + 1) % N;

    //se out = 0, produtoras podem inserir porque o buffer está vazio, já que out percorreu todo o buffer
    if (out == 0) sem_post(&slotVazio); 
    sem_post(&mutexConsumidor);
    return item;
}

void *consumidor (void *threadid) {
  int item;
  int *tid = (int*) threadid;
  printf("Thread consumidora %d comecou a executar.\n", *tid);
  while(1) {
    item = retira(*tid);
    sleep(1);
  }
  free(threadid);
  pthread_exit(NULL);
}

void *produtor (void *threadid) {
  int *tid = (int *) threadid;
  printf("Thread produtora %d comecou a executar.\n", *tid);
  while(1) {
    insere(*tid);
    sleep(1);
  }
  free(threadid);
  pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
  pthread_t threads[NTHREADS];
  int *id[NTHREADS]; 

  //inicia os semaforo usados para exclusão mútua e sincronização condicional
  sem_init(&mutexProdutor, 0, 1); 
  sem_init(&mutexConsumidor, 0, 1); 
  sem_init(&slotCheio, 0, 0); 
  sem_init(&slotVazio, 0, 1); 

  //inicializa os ids das threads
  for (int t=0; t<NTHREADS; t++) {
    if ((id[t] = malloc(sizeof(int))) == NULL) {
       pthread_exit(NULL); return 1;
    }
    *id[t] = t+1;
  }

  //cria as threads consumidoras
  for(int i=0; i<CTHREADS; i++) {
    if (pthread_create(&threads[i], NULL, consumidor, (void *)id[i])) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //cria as threads produtoras
  for(int i=CTHREADS; i<CTHREADS+PTHREADS; i++) {
    if (pthread_create(&threads[i], NULL, produtor, (void *)id[i])) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //espera todas as threads terminarem
  for (int t=0; t<NTHREADS; t++) {
    if (pthread_join(threads[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    }
  }

  pthread_exit(NULL);
}