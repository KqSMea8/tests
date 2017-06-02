package main

import (
	"encoding/json"
	"errors"
	"fmt"
	//"net/http"
)

var StatusErr = errors.New("123")

type Msg struct {
	err error  `json:"err"`
	a   string `json:"a"`
}

type Msg2 struct {
	err string `json:"error"`
	a   string `json:"a"`
}

func test1() {
	msg := Msg{}
	msg.err = StatusErr
	msg.a = "456"
	body, _ := json.Marshal(msg)
	fmt.Println(body)
	fmt.Println(len(body))

	s := string(body)
	fmt.Println(s)

	msgb := Msg{}
	err := json.Unmarshal(body, &msgb)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println(msgb)

}

func test2() {
	msg := Msg2{}
	msg.err = "error"
	msg.a = "456"
	body, _ := json.Marshal(msg)
	fmt.Println(body)
	fmt.Println(len(body))

	s := string(body)
	fmt.Println(s)

	msgb := Msg2{}
	err := json.Unmarshal(body, &msgb)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println(msgb)

}

func main() {
	test1()
	test2()

}
