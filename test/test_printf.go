package main

import "fmt"

func main() {
	str := fmt.Sprintf("%%s %%%dd", 3)
	fmt.Print(str)
}
