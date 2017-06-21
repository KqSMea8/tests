package main

import (
	"fmt"
	"net/url"
)

func main() {
	path := "http://localhost:8080/api/v1/chunks?chunksize=4096&path=.%2Fmain.go&path=.%2Fmain.go"
	m, _ := url.ParseQuery(path)
	fmt.Println(m)

}
