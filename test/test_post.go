package main

import (
	"log"
	"net/http"
)

func main() {
	targetUrl := "http://localhost:8080/api/v1/storage/chunks"
	client := http.Client{}
	req, err := http.NewRequest("POST", targetUrl, http.NoBody)
	if err != nil {
		log.Println(err)
		return
	}

	client.Do(req)
}
