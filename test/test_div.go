package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	fmt.Printf("%d\n", 5/3)

	fi, err := os.Stat("test_div.go")
	if err != nil {
		fmt.Println(err)
		return
	}

	tm1 := fi.ModTime().UnixNano()
	fmt.Printf("%T\n", tm1)

	tm := time.Unix(0, tm1)
	fmt.Println(tm.Format("2006-01-02 15:04:05"))
}
