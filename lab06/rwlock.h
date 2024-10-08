#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void entraLeitura(long int id, pthread_mutex_t mutex, pthread_cond_t cond_leitores);

void saiLeitura(long int id, pthread_mutex_t mutex, pthread_cond_t cond_escritores);

void entraEscrita(long int id, pthread_mutex_t mutex, pthread_cond_t cond_escritores);

void saiEscrita(long int id, pthread_mutex_t mutex, pthread_cond_t cond_leitores, pthread_cond_t cond_escritores);
