package main

import (
	"fmt"
)

type intarr []int

func (p intarr) IsFile() {
	for _, t := range p {
		fmt.Println(t)
	}
}

/*
func (p []int) IsFile() {
}
*/

func main() {

	a := make([]int, 1, 100)
	fmt.Println(a)

	intarr(a).IsFile()
}
