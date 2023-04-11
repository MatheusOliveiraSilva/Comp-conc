#include <stdio.h>
#include <pthread.h>

#define numero_de_threads 2 
#define array_size 10000

int array[array_size];

void init_array() {
    for (int i = 0; i < array_size; i++) {
        array[i] = i;
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

// faz uma função para conferir se o array global foi corretamente multiplicado por 2
void testa_lab1(int *copia_array){
    for (int i = 0; i < array_size; i++) {
        if(array[i] != 2 * copia_array[i]){
            printf("Erro no array na posição %d", i);
            return;
        }
    }
    printf("Array multiplicado por 2 corretamente");
}

int main()
{
    init_array(); // começa o array com valores de 0 a array_size-1
    int copia_array[array_size]; // cria um array para copiar o array global e compararmos depois.
    
    // Copia os elementos do array
    for (int i = 0; i < array_size; i++) {
        copia_array[i] = array[i];
    } 

    pthread_t threads[numero_de_threads]; 

    for (long i = 0; i < numero_de_threads; i++) {
        pthread_create(&threads[i], NULL, print_message, (void *) i);
    }

    for (int i = 0; i < numero_de_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    testa_lab1(copia_array); // compara se array é o dobro de copia_array ( o array original ). 
}