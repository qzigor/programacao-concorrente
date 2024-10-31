package main

import (
	"fmt"
)

func goroutineDialogue(mainToGoroutine chan string, goroutineToMain chan string, done chan bool) {

	msg := <-mainToGoroutine
	fmt.Println("Main envia para Goroutine:", msg)
	goroutineToMain <- "Oi Main, bom dia, tudo bem?"

	msg = <-mainToGoroutine
	fmt.Println("Main envia para Goroutine:", msg)
	goroutineToMain <- "Certo, entendido."

	done <- true
	fmt.Println("Goroutine imprime: finalizando")
}

func main() {

	mainToGoroutine := make(chan string)
	goroutineToMain := make(chan string)
	done := make(chan bool, 1)

	go goroutineDialogue(mainToGoroutine, goroutineToMain, done)

	mainToGoroutine <- "Olá, Goroutine, bom dia!"
	fmt.Println("Goroutine envia para Main:", <-goroutineToMain)

	mainToGoroutine <- "Tudo bem! Vou terminar tá?"
	fmt.Println("Goroutine envia para Main:", <-goroutineToMain)

	<-done
	fmt.Println("Main imprime: finalizando")
}
