#include <stdio.h>
#include <pthread.h>

int leitores = 0;
int escritores = 0;
int escritores_esperando = 0;

void entraLeitura(long int id, pthread_mutex_t mutex, pthread_cond_t cond_leitores){
    pthread_mutex_lock(&mutex);
    while(escritores > 0 || escritores_esperando > 0) {
        printf("Thread %ld esperando para ler.\n", id);
        printf("Leitores: %d Escritores: %d Escritores esperando: %d\n", leitores, escritores, escritores_esperando);
        printf("-------------------------------------\n");
        pthread_cond_wait(&cond_leitores, &mutex);
    }
    leitores++;
    printf("Thread %ld vai ler.\n", id);
    printf("Leitores: %d Escritores: %d Escritores esperando: %d\n", leitores, escritores, escritores_esperando);
    printf("-------------------------------------\n");
    pthread_mutex_unlock(&mutex);
}

void saiLeitura(long int id, pthread_mutex_t mutex, pthread_cond_t cond_escritores){
    pthread_mutex_lock(&mutex);
    leitores--;
    if(leitores==0){
        pthread_cond_signal(&cond_escritores);
    }
    printf("Thread %ld terminou de ler.\n", id);
    printf("Leitores: %d Escritores: %d Escritores esperando: %d\n", leitores, escritores, escritores_esperando);
    printf("-------------------------------------\n");
    pthread_mutex_unlock(&mutex);
}

void entraEscrita(long int id, pthread_mutex_t mutex, pthread_cond_t cond_escritores){
    pthread_mutex_lock(&mutex);
    escritores_esperando++;
    while(leitores > 0 || escritores > 0){
        printf("Thread %ld esperando para escrever.\n", id);
        printf("Leitores: %d Escritores: %d Escritores esperando: %d\n", leitores, escritores, escritores_esperando);
        printf("-------------------------------------\n");
        pthread_cond_wait(&cond_escritores, &mutex);
    }
    escritores++;
    escritores_esperando--;
    printf("Thread %ld vai escrever.\n", id);
    printf("Leitores: %d Escritores: %d Escritores esperando: %d\n", leitores, escritores, escritores_esperando);
    printf("-------------------------------------\n");
    pthread_mutex_unlock(&mutex);
}

void saiEscrita(long int id, pthread_mutex_t mutex, pthread_cond_t cond_leitores, pthread_cond_t cond_escritores){
    pthread_mutex_lock(&mutex);
    escritores--;
    pthread_cond_signal(&cond_escritores);
    pthread_cond_broadcast(&cond_leitores);
    printf("Thread %ld terminou de escrever.\n", id);
    printf("Leitores: %d Escritores: %d Escritores esperando: %d\n", leitores, escritores, escritores_esperando);
    printf("-------------------------------------\n");
    pthread_mutex_unlock(&mutex);
}   
