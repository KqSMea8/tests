package main

import "path/filepath"
import "fmt"

func main() {
	path := `a///b///c///d`
	path = filepath.FromSlash(path) // 平台处理

	d1 := filepath.Dir(path)
	f1 := filepath.Base(path)
	d2, f2 := filepath.Split(path)
	fmt.Printf("%q  %q\n%q  %q\n", d1, f1, d2, f2)
	// "a/b/c"  "d"
	// "a///b///c///"  "d"

	ext := filepath.Ext(path)
	fmt.Println(ext) // .txt

	path = "/pfs/datacenter/dir1"
	dir, file := filepath.Split(path)
	fmt.Printf("dir:%s file:%s\n", dir, file)
}
