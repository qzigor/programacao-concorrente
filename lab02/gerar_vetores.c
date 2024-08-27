#include<stdio.h>
#include<stdlib.h>
#include<time.h>

float *createRandomVector(int n);
double innerProduct(float *vector1, float *vector2, int size);

int main(int argc, char*argv[]){
    int size;
    double inner_product;
    FILE *file;
    size_t ret;

    if(argc < 3){
        printf("ERRO: Informe o tamanho do vetor e o arquivo de saida\n");
        return 1;
    }

    size = atoi(argv[1]);
    srand(time(NULL));
    float *vector1 = createRandomVector(size);
    float *vector2 = createRandomVector(size);
    inner_product = innerProduct(vector1, vector2, size);

    file = fopen(argv[2], "wb");
    if(!file){
        printf("Erro ao abrir o arquivo %s.\n", argv[2]);
        return 2;
    }
    ret = fwrite(&size, sizeof(int), 1, file);
    ret = fwrite(vector1, sizeof(float), size, file);
    if(ret < size) {
      printf("Erro de escrita no  arquivo\n");
      return 3;
    }
    ret = fwrite(vector2, sizeof(float), size, file);
    if(ret < size) {
      printf("Erro de escrita no  arquivo\n");
      return 4;
    }
    ret = fwrite(&inner_product, sizeof(double), 1, file);
    
    fclose(file);
    free(vector1);
    free(vector2);

    return 0;
}

float *createRandomVector(int n){
    int i;
    float initial = 1.0;
    float end = 1000000.0;
    float *vector, element;
    vector = (float*) malloc(sizeof(float) * n);
    if(!vector) {
        printf("Erro de alocao da memoria do vetor\n");
        return NULL;
    }
    for(i=0; i<n; i++){
        element = initial + (float)rand()/RAND_MAX * (end - initial);
        vector[i] = element;
    }
    return vector;
}

double innerProduct(float *vector1, float *vector2, int size){
    int i;
    double inner_product = 0;

    for(i=0; i<size; i++){
        inner_product += vector1[i] * vector2[i];
    }

    return inner_product;
}
