#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//#define TEXTO

int main(int argc, char*argv[]){
    float *matrix;
    int rows, columns;
    long long int dimension;
    int i;
    FILE *file;
    size_t ret;
    
    if(argc < 4){
        printf("Informe <linhas> <colunas> <arquivo de saida>\n");
        return 1;
    }
    rows = atoi(argv[1]);
    columns = atoi(argv[2]);
    dimension = rows * columns;

    matrix = (float *)malloc(sizeof(float)*dimension);
    if(matrix == NULL){
        printf("Erro ao alocar espaco para a matriz\n");
        return 2;
    }
    srand(time(NULL));
    for(i=0; i<dimension; i++) {
        *(matrix+i) = (rand() % 1000) * 0.3;
    }

    #ifdef TEXTO
    for(int i=0; i<rows; i++) {
        for(int j=0; j<columns; j++)
            fprintf(stdout, "%.6f ", matrix[i*columns+j]);
        fprintf(stdout, "\n");
    }
    #endif

    file = fopen(argv[3], "wb");
    if(!file){
        printf("Erro na abertura do arquivo.\n");
        return 3;
    }
    ret = fwrite(&rows, sizeof(int), 1, file);
    ret = fwrite(&columns, sizeof(int), 1, file);
    ret = fwrite(matrix, sizeof(float), dimension, file);
    if(ret < dimension) {
        printf("Erro de escrita no  arquivo\n");
        return 4;
    }

    fclose(file);
    free(matrix);
    return 0;
}
