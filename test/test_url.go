package main

import "fmt"
import "net/url"

func main() {
	path := "http://localhost:8080/api/v1/chunks?chunksize=4096&path=.%2Fmain.go&path=.%2Fmain.go"
	url.ParseQuery(path)

}
