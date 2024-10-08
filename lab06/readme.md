# Sistema de Leitores e Escritores com Prioridade para Escritores

## Descrição
Este projeto implementa um sistema concorrente de controle de acesso a uma lista de inteiros compartilhada, onde múltiplas threads realizam operações de leitura e escrita simultaneamente. O objetivo principal é garantir prioridade para escritores em cenários de concorrência, utilizando mutexes e variáveis de condição (pthread_mutex_t e pthread_cond_t).

## Compilar o programa
Para compilar o código, você precisará de um compilador compatível com C e que ofereça suporte para o uso de threads. Certifique-se de ter as bibliotecas pthread instaladas em seu sistema. A compilação pode ser feita no terminal usando o seguinte comando:<br>
``` bash
gcc -o nome_do_programa main_rwlock.c list_int.c rwlock.c -lpthread
```

## Saída do programa
A saída do programa exibe informações detalhadas sobre as operações realizadas pelas threads, tanto de leitura quanto de escrita, além de fornecer um resumo do tempo total de execução.

Exemplo de saída:<br>
Thread 2 esperando para ler.<br>
Leitores: 0 Escritores: 1 Escritores esperando: 2

O campo leitores indica a quantidade de leitores ativos, o campo escritores indica a quantidade de escritores e o campo escritores esperando indica a quantidade de escritores que estão na fila para escrever.
