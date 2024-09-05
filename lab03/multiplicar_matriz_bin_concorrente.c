#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
//#define TEXTO
#define TIME

float *first_matrix;
float *second_matrix;
float *result_matrix;
int nthreads;

typedef struct{
    int thread_id;
    int first_rows, first_columns, second_columns;
} tArgs;

void *multiplyMatrix(void *arg){
    tArgs *args = (tArgs*) arg;
    int i, j, l;

    // Cada thread calcula parte das linhas da matriz C
    for (i = args->thread_id; i < args->first_rows; i += nthreads) {
        for (j = 0; j < args->second_columns; j++) {
            float sum = 0;
            for (l = 0; l < args->first_columns; l++) {
                sum += first_matrix[i * args->first_columns + l] * second_matrix[l * args->second_columns + j];
            }
            result_matrix[i * args->second_columns + j] = sum;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    FILE *first_file, *second_file, *result_file;
    size_t ret;
    pthread_t *tid;
    tArgs *args;
    int i, j;
    int first_rows, first_columns, second_rows, second_columns, result_rows, result_columns;
    long long int first_dimension, second_dimension, result_dimension;
    clock_t start, end;
    double startup_time, multiplication_time, end_time;

    start = clock();
    if(argc < 4){
        printf("Informe: <arquivo da matriz01> <arquivo da matriz02> <arquivo de saida> <nthreads>.\n");
        return 1;
    }
    nthreads = atoi(argv[4]);

    first_file = fopen(argv[1], "rb");
    if(!first_file){
        printf("Erro ao abrir arquivo da primeira matriz.\n");
        return 2;
    }
    ret = fread(&first_rows, sizeof(int), 1, first_file);
    if(!ret) {
        printf("Erro de leitura das linhas da primeira matriz.\n");
        return 2;
    }
    ret = fread(&first_columns, sizeof(int), 1, first_file);
    if(!ret) {
        printf("Erro de leitura das colunas da primeira matriz.\n");
        return 2;
    }
    first_dimension = first_rows * first_columns;
    first_matrix = (float *)malloc(sizeof(float)*first_dimension);
    if(!first_matrix){
        printf("Erro na alocacao da primeira matriz.\n");
        return 2;
    }
    ret = fread(first_matrix, sizeof(float), first_dimension, first_file);
    if(ret < first_dimension) {
        printf("Erro de leitura dos elementos da primeira matriz\n");
        return 2;
    }
    fclose(first_file);

    second_file = fopen(argv[2], "rb");
    if(!second_file){
        printf("Erro ao abrir arquivo da segunda matriz.\n");
        return 3;
    }
    ret = fread(&second_rows, sizeof(int), 1, second_file);
    if(!ret) {
        printf("Erro de leitura das linhas da segunda matriz.\n");
        return 3;
    }
    ret = fread(&second_columns, sizeof(int), 1, second_file);
    if(!ret) {
        printf("Erro de leitura das colunas da segunda matriz.\n");
        return 3;
    }
    second_dimension = second_rows * second_columns;
    second_matrix = (float *)malloc(sizeof(float)*second_dimension);
    if(!second_matrix){
        printf("Erro na alocacao da segunda matriz.\n");
        return 3;
    }
    ret = fread(second_matrix, sizeof(float), second_dimension, second_file);
    if(ret < second_dimension) {
        printf("Erro de leitura dos elementos da segunda matriz\n");
        return 3;
    }
    fclose(second_file);
    end = clock();
    startup_time = ((double)(end - start)/CLOCKS_PER_SEC);

    start = clock();
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid == NULL) {
        puts("Erro na alocacao das threads"); 
        return 4;
    }
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args == NULL){
        printf("Erro na alocacao das estruturas das threads.\n");
        return 4;
    }

    if(first_columns != second_rows){
        printf("Nao e possivel multiplicar essas matrizes.\n");
        return 5;
    }
    result_rows = first_rows;
    result_columns = second_columns;
    result_dimension = result_rows * result_columns;

    result_matrix = (float *)malloc(sizeof(float)*result_dimension);
    if(!result_matrix){
        printf("Erro na alocacao da matriz resultado.\n");
        return 6;
    }

    for(i=0; i < result_rows; i++){
        for(j=0; j < result_columns; j++){
            result_matrix[i * result_columns + j] = 0;
        }
    }

    for (i = 0; i < nthreads; i++){
        (args+i)->thread_id = i;
        (args+i)->first_rows = first_rows;
        (args+i)->first_columns = first_columns;
        (args+i)->second_columns = second_columns;
        pthread_create(tid+i, NULL, multiplyMatrix, (void*)(args+i));
    }

    for(int i = 0; i < nthreads; i++) {
      pthread_join(*(tid+i), NULL);
    }
    end = clock();
    multiplication_time = ((double)(end - start))/CLOCKS_PER_SEC;


    start = clock();
    result_file = fopen(argv[3], "wb");
    if(!result_file){
        printf("Erro na abertura do arquivo.\n");
        return 7;
    }
    ret = fwrite(&result_rows, sizeof(int), 1, result_file);
    ret = fwrite(&result_columns, sizeof(int), 1, result_file);
    ret = fwrite(result_matrix, sizeof(float), result_dimension, result_file);
    if(ret < result_dimension) {
        printf("Erro de escrita no  arquivo\n");
        return 7;
    }
    fclose(result_file);

    #ifdef TEXTO
    printf("Primeira matriz:\n");
    for(i=0; i<first_rows; i++){
        for(j=0; j<first_columns; j++){
            printf("%.5f  ", first_matrix[i*first_columns+j]);
        }
        printf("\n");
    }
    printf("\nSegunda matriz:\n");
    for(i=0; i<second_rows; i++){
        for(j=0; j<second_columns; j++){
            printf("%.5f  ", second_matrix[i*second_columns+j]);
        }
        printf("\n");
    }
    printf("\nResultado:\n");
    for(i=0; i<result_rows; i++){
        for(j=0; j<result_columns; j++){
            printf("%.5f  ", result_matrix[i*result_columns+j]);
        }
        printf("\n");
    }
    #endif

    free(first_matrix);
    free(second_matrix);
    free(result_matrix);
    free(args);
    end = clock();
    end_time = ((double)(end - start))/CLOCKS_PER_SEC;

    #ifdef TIME
        printf("Tempo de inicializacao: %.10lf\n", startup_time);
        printf("Tempo de multiplicacao: %.10lf\n", multiplication_time);
        printf("Tempo de fincalizacao: %.10lf\n", end_time);
    #endif

    return 0;

}
