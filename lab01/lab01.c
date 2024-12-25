#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *initializeVector(int N);

void toCheck(int *vector, int N);
typedef struct{
    int *vector;
    int initial_index, end_index;
} threadData;


void *Increment(void *args){
    int i;
    threadData *data = (threadData*) args;
    for(i=data->initial_index; i<data->end_index; i++){
        data->vector[i] += 1;
    }
    free(data);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int nthreads, vector_size, i;
    int start=0, end=0;

    if(argc < 3){
      printf("--ERRO: informe a qtde de threads <%s> <nthreads>\n", argv[0]);
      return 1;
    }
    nthreads = atoi(argv[1]);
    vector_size = atoi(argv[2]);

    int *vector = initializeVector(vector_size);
    
    pthread_t threads[nthreads];
    threadData *data;
    int elements_per_thread = vector_size / nthreads;
    int extra_elements = vector_size % nthreads;

    for (i = 0; i < nthreads; i++) {
        data = malloc(sizeof(threadData));
        if(data == NULL){
            printf("Erro no malloc\n");
            return 1;
        }

        data->initial_index = start;
        if(extra_elements != 0){
            end += elements_per_thread + 1;
            start += elements_per_thread + 1;
            extra_elements--;
        }
        else{
            end += elements_per_thread;
            start += elements_per_thread;
        }

        data->end_index = end;
        data->vector = vector;

        pthread_create(&threads[i], NULL, Increment, (void*)data);
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
    }

    toCheck(vector, vector_size);

    free(vector);

    return 0;
}

int *initializeVector(int N){
    int i;
    int *vector = (int*)malloc(N * sizeof(int));

    if (vector == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }

    for (i=0; i<N; i++){
        vector[i] = i;
    }

    return vector;
}

void toCheck(int *vector, int N){
    int *auxiliary_vector = initializeVector(N);
    int i;

    for(i=0; i<N; i++){
        if(vector[i] == auxiliary_vector[i] + 1){
            continue;
        }
        else{
            printf("Erro no incremento.\n");
        }
    }
}
