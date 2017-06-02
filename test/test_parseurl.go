package main

import (
	"fmt"
	"net/url"
)

func main() {
	m, err := url.ParseQuery(`x=1&y=2&y=3;z`)
	if err != nil {
		fmt.Println(err)
	}

	fmt.Println(m)

	a := int64(111111111111111111)
	fmt.Printf("%d", a)
}
