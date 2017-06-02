package main

import (
	"log"
	"os"
	//sys "syscall"
)

func create2() {
	//size := int64(10 * 1024 * 1024)
	fd, err := os.OpenFile("output", os.O_RDWR, 0666)
	if err != nil {
		log.Fatal("Failed to create output")
	}
	defer fd.Close()

	_, err = fd.Seek(1, 0)
	if err != nil {
		log.Fatal("seek error")
	}
	size := int64(10 * 1024 * 1024)
	_, err = fd.Seek(size, 0)
	if err != nil {
		log.Fatal("Failed to seek")
	}
	_, err = fd.Write([]byte{0})
	if err != nil {
		log.Fatal("Write failed")
	}
}

func create() {
	size := int64(10 * 1024 * 1024)
	fd, err := os.Create("./tmp1/tmp2/tmp3/output")
	if err != nil {
		log.Fatal("Failed to create output")
	}
	_, err = fd.Seek(size-1, 0)
	if err != nil {
		log.Fatal("Failed to seek")
	}
	_, err = fd.Write([]byte{0})
	if err != nil {
		log.Fatal("Write failed")
	}
	err = fd.Close()
	if err != nil {
		log.Fatal("Failed to close file")
	}
}

func open() {
	size := int64(10 * 1024)

	fd, err := os.Open("output2")
	if err != nil {
		log.Fatal("Failed to create output")
	}
	defer fd.Close()

	_, err = fd.Seek(size-1, 0)
	if err != nil {
		log.Fatal("Failed to seek")
	}
	_, err = fd.Write([]byte{0})
	if err != nil {
		log.Fatal("Write failed")
	}
	err = fd.Close()
	if err != nil {
		log.Fatal("Failed to close file")
	}
}

func main() {
	//open()
	//:wqa
	create()
	//create2()

}
