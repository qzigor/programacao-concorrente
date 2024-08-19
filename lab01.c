#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

typedef struct{
    int *vector;
    int initial_index, end_index;
} threadData;

int *initializeVector(int N);

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

    if(argc < 3){
      printf("--ERRO: informe a qtde de threads <%s> <nthreads>\n", argv[0]);
      return 1;
    }
    nthreads = atoi(argv[1]);
    vector_size = atoi(argv[2]);

    int *vector = initializeVector(vector_size);
    for(i=0; i<vector_size; i++){
        printf("%d ", vector[i]);
    }
    printf("\n");
    
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
        int start = i * elements_per_thread + (i < extra_elements ? i : extra_elements);
        int end = start + elements_per_thread + (i < extra_elements ? 1 : 0);

        data->vector = vector;
        data->initial_index = start;
        data->end_index = end;

        pthread_create(&threads[i], NULL, Increment, (void*)data);
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
    }

    for (i=0; i<vector_size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");

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
