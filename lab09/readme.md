# Projeto: Comunicação entre Goroutines e Contagem de Números Primos

Este projeto em Go explora o uso de goroutines e canais em dois programas principais: um diálogo simples entre goroutines e uma contagem concorrente de números primos.
Funcionalidades

## Diálogo entre Goroutines
Demonstra comunicação entre a função main e uma goroutine secundária usando canais. A main envia mensagens para a goroutine, que responde de volta, finalizando o diálogo.

## Contagem de Números Primos
Conta números primos de 1 a N utilizando M goroutines, que verificam e somam quantos números primos existem no intervalo, comunicando o resultado total à função main.

## Como Executar

Instale o Go.
Compile e execute os arquivos com:
```
go run <nome_do_arquivo>.go
```
