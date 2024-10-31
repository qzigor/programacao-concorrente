package main

import (
	"fmt"
	"math"
	"sync"
)

func isPrime(n int) bool {
	if n <= 1 {
		return false
	}
	if n == 2 {
		return true
	}
	if n%2 == 0 {
		return false
	}
	for i := 3; i <= int(math.Sqrt(float64(n)))+1; i += 2 {
		if n%i == 0 {
			return false
		}
	}
	return true
}

func primeWorker(numbers <-chan int, results chan<- int, wg *sync.WaitGroup) {
	defer wg.Done()
	count := 0
	for num := range numbers {
		if isPrime(num) {
			count++
		}
	}
	results <- count
}

func main() {
	N := 100
	M := 4

	numbers := make(chan int, N)
	results := make(chan int, M)
	var wg sync.WaitGroup

	for i := 0; i < M; i++ {
		wg.Add(1)
		go primeWorker(numbers, results, &wg)
	}

	go func() {
		for i := 1; i <= N; i++ {
			numbers <- i
		}
		close(numbers)
	}()

	go func() {
		wg.Wait()
		close(results)
	}()

	totalPrimes := 0
	for count := range results {
		totalPrimes += count
	}

	fmt.Printf("Quantidade total de números primos de 1 a %d: %d\n", N, totalPrimes)
}
