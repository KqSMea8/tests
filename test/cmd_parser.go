package main

import (
	//"fmt"
	"github.com/galdor/go-cmdline"
	"os"
)

/*
func main() {
	for i, arg := range os.Args {
		if arg == "-help" {
			fmt.Printf("I need somebody\n")
		} else if arg == "-version" {
			fmt.Printf("Version Zero\n")
		} else {
			fmt.Printf("arg %d: %s\n", i, os.Args[i])
		}
	}
}
*/

func main() {
	cmdline := cmdline.New()

	cmdline.AddFlag("v", "verbose", "log more information")
	cmdline.AddOption("o", "output", "file", "the output file")
	cmdline.SetOptionDefault("output", "-")

	/*
		args := cmdline.Parse(os.Args)
		for _, arg := range args {
			fmt.Println(arg)
		}
	*/

	//cmdline.PrintUsage(os.Stdout)

	for _, v := cmdline.Arguments {
		fmt.Println(v)
	}
}
