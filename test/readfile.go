package main

import (
	"os"
)

func main() {
	f, err := os.Open("/pfs/datacent/1.txt") // For read access.
	if err != nil {
		return
	}

	defer f.Close()
	return
}
