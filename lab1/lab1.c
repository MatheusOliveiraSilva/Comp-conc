#include <stdio.h>
#include <pthread.h>

#define numero_de_threads 2 
#define array_size 101

int array[array_size];

void init_array() {
    printf("Array inicial: ");
    for (int i = 0; i < array_size; i++) {
        array[i] = i;
        printf("%d ", array[i]);
    }   
    printf("\n");
}

void *print_message(void *thread_id) {
    
    if(!(thread_id)){ // se for a thread 0 
        for (int i = 0; i < array_size/2; i++) {
            array[i] = 2 * array[i];
        }
    }else{ // se for a thread 1
        for (int i = array_size/2; i < array_size; i++) {
            array[i] = 2 * array[i];
        }
    }
    pthread_exit(NULL);
}

int main()
{
    init_array(); // começa o array com valores de 0 a array_size-1
    pthread_t threads[numero_de_threads]; 

    for (long i = 0; i < numero_de_threads; i++) {
        pthread_create(&threads[i], NULL, print_message, (void *) i);
    }

    for (int i = 0; i < numero_de_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n\nArray final multiplicado por 2: ");
    for (int i = 0; i < array_size; i++) {
        printf("%d ", array[i]);
    }
}