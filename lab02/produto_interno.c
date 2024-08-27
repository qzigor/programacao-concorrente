#include<stdio.h>
#include<stdlib.h> 
#include<pthread.h>
#include<math.h>

typedef struct {
    float *vector1;
    float *vector2;
    int start;
    int end;
    double partial_sum;
} ThreadData;

void *innerProduct(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->partial_sum = 0;

    for (int i = data->start; i < data->end; i++) {
        data->partial_sum += data->vector1[i] * data->vector2[i];
    }

    return NULL;
}

int main(int argc, char*argv[]){
    FILE *file;
    size_t ret;
    int size_vector, nthreads, chunk_size, i;
    float *vector1, *vector2;
    double inner_product_initial, inner_product_new, variation;

    if(argc < 3){
        printf("Erro: informe o numero de threads e o nome do arquivo.\n");
        return 1;
    }
    file = fopen(argv[2], "rb");
    if(file == NULL){
        printf("Erro: abrir arquivo.\n");
        return 2;
    }
    ret = fread(&size_vector, sizeof(int), 1, file);
    if(!ret){
        printf("Erro: leitura da dimensao do vetor.\n");
        return 3;
    }
    vector1 = malloc(sizeof(float) * size_vector);
    if(vector1 == NULL){
        printf("Erro: alocacao do primeiro vetor.\n");
        return 4;
    }
    ret = fread(vector1, sizeof(float), size_vector, file);
    if(ret < size_vector){
        printf("Erro: leitura dos elementos do primeiro vetor.\n");
        return 5;
    }
    vector2 = malloc(sizeof(float) * size_vector);
    if(vector2 == NULL){
        printf("Erro: alocacao do segundo vetor.\n");
        return 6;
    }
    ret = fread(vector2, sizeof(float), size_vector, file);
    if(ret < size_vector){
        printf("Erro: leitura dos elementos do segundo vetor.\n");
        return 7;
    }
    ret = fread(&inner_product_initial, sizeof(double), 1, file);

    nthreads = atoi(argv[1]);
    pthread_t threads[nthreads];
    ThreadData thread_data[nthreads];
    chunk_size = size_vector / nthreads;

    for(i=0; i<nthreads; i++){
        thread_data[i].vector1 = vector1;
        thread_data[i].vector2 = vector2;
        thread_data[i].start = i*chunk_size;
        thread_data[i].end = (i == nthreads - 1) ? size_vector : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, innerProduct, (void *)&thread_data[i]);
    }

    for (i = 0; i < nthreads; i++) {
        pthread_join(threads[i], NULL);
        inner_product_new += thread_data[i].partial_sum;
    }

    variation = fabs((inner_product_initial - inner_product_new)/inner_product_initial);
    printf("Variacao do produto interno: %.25lf", variation);

    free(vector1);
    free(vector2);
    fclose(file);
    return 0;
}

/*
    Nos arquivos teste, foi possível perceber que quando maior a quantidade de elementos em um vetor, ocorria uma tendência em apresentar mais erros na variação entre o produto interno sequêncial e o concorrente.
*/