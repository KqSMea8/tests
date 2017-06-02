package main

import (
	"fmt"
	"os"
	"path/filepath"
)

func main() {
	path := "/tmp/"
	r := true
	filepath.Walk(path, func(subpath string, info os.FileInfo, err error) error {
		//log.Println("path:\t" + path)

		/*
			m := FileMeta{}
			m.Path = info.Name()
			m.Size = info.Size()
			m.ModTime = info.ModTime().Format("2006-01-02 15:04:05")
			m.IsDir = info.IsDir()
			metas = append(metas, m)
		*/

		if subpath != path {
			fmt.Println(subpath)
		}

		if info.IsDir() && !r && subpath != path {
			return filepath.SkipDir
		}

		//log.Println(len(metas))
		return nil
	})
}
