//Programa concorrente que cria e faz operacoes sobre uma lista de inteiros

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "list_int.h"
#include "rwlock.h"

#define QTDE_OPS 100 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 10 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido

//lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 
//qtde de threads no programa
int nthreads;

pthread_mutex_t mutex;
pthread_cond_t cond_leitores, cond_escritores;

//tarefa das threads
void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read; 
   in=out=read = 0; 

   //realiza operacoes de consulta (70%), insercao (29%) e remocao (1%)
   for(long int i=id; i<QTDE_OPS; i+=nthreads) {
      op = rand() % 100;
      if(op<70) {
	      entraLeitura(id, mutex, cond_leitores); /* lock de LEITURA */    
         Member(i%MAX_VALUE, head_p);   /* Ignore return value */
	      saiLeitura(id, mutex, cond_escritores);    
	      read++;
      } else if(70<=op && op<99) {
	      entraEscrita(id, mutex, cond_escritores); /* lock de ESCRITA */    
         Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
	      saiEscrita(id, mutex, cond_leitores, cond_escritores);     
	      in++;
      } else if(op>=99) {
	      entraEscrita(id, mutex, cond_escritores);  /* lock de ESCRITA */     
         Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
	      saiEscrita(id, mutex, cond_leitores, cond_escritores);     
	      out++;
      }
   }
   //registra a qtde de operacoes realizadas por tipo
   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   pthread_t *tid;
   clock_t start, end;
   double time;
   
   //verifica se o numero de threads foi passado na linha de comando
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
   }
   nthreads = atoi(argv[1]);

   //insere os primeiros elementos na lista
   for(int i=0; i<QTDE_INI; i++)
      Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
   

   //aloca espaco de memoria para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {  
      printf("--ERRO: malloc()\n"); return 2;
   }

   //tomada de tempo inicial
   start = clock();
   //inicializa a variavel mutex
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&cond_leitores, NULL);
   pthread_cond_init(&cond_escritores, NULL);

   //cria as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); 
         return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); 
         return 4;
      }
   }

   //tomada de tempo final
   end = clock();
   time = ((double)(end-start)/CLOCKS_PER_SEC);
   printf("Tempo: %lf\n", time);

   //libera o mutex
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_escritores);
   pthread_cond_destroy(&cond_leitores);

   //libera o espaco de memoria do vetor de threads
   free(tid);
   //libera o espaco de memoria da lista
   Free_list(&head_p);

   return 0;
}  /* main */
