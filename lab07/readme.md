# Processamento Concorrente de Arquivo com Threads e Semáforos

Este projeto é um programa em C que utiliza programação concorrente com threads e semáforos para ler, processar e salvar dados de um arquivo de texto. O programa envolve três threads que executam as seguintes tarefas.<br>
## Funcionalidade

Thread 1 (carregar): Lê um arquivo de entrada de tamanho arbitrário e armazena seu conteúdo no buffer1, redimensionando o buffer conforme necessário.

Thread 2 (processar): Processa os caracteres do buffer1, adicionando um caractere de nova linha (\n) após uma quantidade crescente de caracteres (de acordo com a fórmula 2n+1, onde n=0,1,2,...,10). Após n=10, a thread insere uma nova linha a cada 10 caracteres lidos e os armazena em buffer2.

Thread 3 (exibir): Escreve o conteúdo processado de buffer2 em um arquivo de saída.

## Como Compilar

Execute o comando abaixo no terminal para compilar o programa:
```
gcc -o semafaro semafaro.c -lpthread
```

## Como Executar

Após compilar, execute o programa passando dois argumentos: o nome do arquivo de entrada e o nome do arquivo de saída:
```
./semafaro entrada.txt saida.txt
```
entrada.txt: O arquivo de texto que será lido e processado.<br>
saida.txt: O arquivo de texto onde o resultado processado será salvo.
