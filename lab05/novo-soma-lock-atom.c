#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

long int soma = 0;
pthread_mutex_t mutex;
pthread_cond_t cond_tarefa, cond_extra;

int imprimir = 0;
int contador_multiplos = 0;

void *ExecutaTarefa(void *arg) {
    printf("Thread de Tarefa esta executando...\n");

    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        soma++;
        if (soma % 10 == 0 && contador_multiplos != 20) {
            imprimir = 1;
            pthread_cond_signal(&cond_extra); 
            pthread_cond_wait(&cond_tarefa, &mutex); 
        }
        pthread_mutex_unlock(&mutex);
    }

    printf("Thread de Tarefa terminou!\n");
    pthread_exit(NULL);
}

void *extra(void *args) {
    printf("Thread Extra esta executando...\n");

    while (contador_multiplos < 20) { 
        pthread_mutex_lock(&mutex);

        while (!imprimir) { 
            pthread_cond_wait(&cond_extra, &mutex);
        }
        printf("soma = %ld\n", soma);
        contador_multiplos++;
        imprimir = 0;
        pthread_cond_signal(&cond_tarefa);
        pthread_mutex_unlock(&mutex);
    }

    printf("Thread Extra terminou!\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t tarefa_thread, extra_thread;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_tarefa, NULL);
    pthread_cond_init(&cond_extra, NULL);

    if (pthread_create(&tarefa_thread, NULL, ExecutaTarefa, NULL)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    if (pthread_create(&extra_thread, NULL, extra, NULL)) {
        printf("--ERRO: pthread_create()\n");
        exit(-1);
    }

    if (pthread_join(tarefa_thread, NULL)) {
        printf("--ERRO: pthread_join() \n");
        exit(-1);
    }
    if (pthread_join(extra_thread, NULL)) {
        printf("--ERRO: pthread_join() \n");
        exit(-1);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_tarefa);
    pthread_cond_destroy(&cond_extra);

    printf("Valor final de 'soma' = %ld\n", soma);

    return 0;
}
