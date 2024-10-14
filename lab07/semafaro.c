#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

char *buffer1 = NULL;
char *buffer2 = NULL;
int buffer1_size;
int buffer2_size;
int chunk = 256;

sem_t sem_thread2, sem_thread3;

void *carregar(void *arg){
    char* entrada = (char*)arg;
    FILE* arquivo = fopen(entrada, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        pthread_exit(NULL);
    }

    buffer1_size = 0;
    int capacidade_buffer = chunk;

    buffer1 = (char*)malloc(capacidade_buffer * sizeof(char));
    if (!buffer1) {
        printf("Erro ao alocar memória para buffer1");
        fclose(arquivo);
        pthread_exit(NULL);
    }

    int lido;
    while ((lido = fread(buffer1 + buffer1_size, sizeof(char), chunk, arquivo)) > 0) {
        buffer1_size += lido;
        if (buffer1_size + chunk > capacidade_buffer) {
            capacidade_buffer *= 2;
            buffer1 = (char*)realloc(buffer1, capacidade_buffer * sizeof(char));
            if (!buffer1) {
                printf("Erro ao realocar memória para buffer1");
                fclose(arquivo);
                pthread_exit(NULL);
            }
        }
    }

    fclose(arquivo);

    sem_post(&sem_thread2);
    pthread_exit(NULL);
}

void *processar(void *arg){
    sem_wait(&sem_thread2);

    buffer2_size = 0;
    int capacidade_buffer2 = buffer1_size;

    buffer2 = (char*)malloc(capacidade_buffer2 * sizeof(char));
    if (!buffer2) {
        perror("Erro ao alocar memória para buffer2");
        pthread_exit(NULL);
    }

    int i = 0;
    int j = 0;
    int n = 0;
    int contador = 0;

    while (i < buffer1_size) {
        buffer2[j++] = buffer1[i++];
        contador++;

        if (n <= 10) {
            if (contador == (2 * n + 1)) {
                buffer2[j++] = '\n';
                contador = 0;
                n++;
            }
        } else {
            if (contador == 10) {
                buffer2[j++] = '\n';
                contador = 0;
            }
        }
  
        if (j + 1 > capacidade_buffer2) {
            capacidade_buffer2 *= 2;
            buffer2 = (char*)realloc(buffer2, capacidade_buffer2 * sizeof(char));
            if (!buffer2) {
                perror("Erro ao realocar memória para buffer2");
                pthread_exit(NULL);
            }
        }
        
    }

    buffer2[j] = '\0';
    buffer2_size = j;

    sem_post(&sem_thread3);
    pthread_exit(NULL);
}

void *exibir(void *arg){
    sem_wait(&sem_thread3);

    char* nome_arquivo_saida = (char*)arg;
    FILE* arquivo_saida = fopen(nome_arquivo_saida, "w");
    if (arquivo_saida == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        pthread_exit(NULL);
    }

    fwrite(buffer2, sizeof(char), buffer2_size, arquivo_saida);
    
    fclose(arquivo_saida);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){

    if (argc < 3) {
        fprintf(stderr, "Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    sem_init(&sem_thread2, 0, 0);
    sem_init(&sem_thread3, 0, 0);

    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, carregar, argv[1]);
    pthread_create(&t2, NULL, processar, NULL);
    pthread_create(&t3, NULL, exibir, argv[2]);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    free(buffer1);
    free(buffer2);

    sem_destroy(&sem_thread2);
    sem_destroy(&sem_thread3);

    return 0;
    
}
