package main

import (
	"encoding/json"
	"errors"
	"fmt"

	sjson "github.com/bitly/go-simplejson"
)

type Option struct {
	Name  string `json:"Name"`
	Value string `json:"Value"`
}

type CmdAttr struct {
	Method string `json:"method"`
	//Options []Option `json:"options"`
	//Args    []string `json:"args"`
}

type Msg2 struct {
	Err2 error       `json:"error2"`
	A    string      `json:"a2"`
	B    interface{} `json:"B"`
}

type MsgResult struct {
	A string `json:"a"`
	B string `json:"b"`
}

type Msg3 struct {
	Err    int32       `json:"status""`
	Result interface{} `json:"result"`
}

/*
func test1() {
	attr := CmdAttr{}

	attr.Method = "touch"
	attr.Options = make([]Option, 0, 100)

	option := Option{}
	option.Name = "file-size"
	option.Value = "100"

	attr.Args = make([]string, 0, 100)
	attr.Args = append(attr.Args, "/tmp/100.txt")

	attr.Options = append(attr.Options, option)

	data, _ := json.Marshal(attr)

	fmt.Println(string(data[:]))

}
*/

var StatusErr = errors.New("123")

func test2() {
	msg := Msg2{}
	msg.Err2 = StatusErr
	msg.A = "456"
	attr := CmdAttr{Method: "ls"}
	msg.B = []CmdAttr{attr}

	body, _ := json.Marshal(&msg)
	fmt.Println(string(body[:]))

	msgb := Msg2{}
	msgb.B = []CmdAttr{}
	err := json.Unmarshal(body, &msgb)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Println(msgb)

	fmt.Printf("%T\n", msgb.B)
	//fmt.Printf("%#v\n", msgb.B)
	fmt.Printf("%#v\n", msgb.B)
}

func test3() {
	msg := Msg3{}
	msg.Err = -1
	msg.Result = &MsgResult{
		A: "a",
		B: "b",
	}

	fmt.Printf("result type:%T\n", msg.Result)
	fmt.Printf("result type:%v\n", msg.Result.(*MsgResult))
	a := msg.Result.(*MsgResult)
	fmt.Println(a)
	body, _ := json.Marshal(&msg)
	fmt.Println(string(body[:]))

	fmt.Println(body)

	msgb := Msg3{}
	msgb.Result = &MsgResult{}
	json, _ := sjson.NewJson(body)
	//fmt.Println(msgb)
	//fmt.Println(json.Get("result"))
	//fmt.Println(json.Get("status"))
	result := json.Get("result")
	fmt.Println(result)

	arr, _ := json.Bytes()
	fmt.Println(arr)
}

func main() {

	//test1()
	test2()
	//test3()
}
