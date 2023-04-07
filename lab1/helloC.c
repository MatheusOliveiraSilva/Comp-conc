#include <stdio.h>
#include <pthread.h>

void *print_message(void *thread_id) {
  printf("Hello, World! from thread %ld\n", (long) thread_id);
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[10];

  for (long i = 0; i < 10; i++) {
    pthread_create(&threads[i], NULL, print_message, (void *) i);
  }

  for (int i = 0; i < 10; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}

