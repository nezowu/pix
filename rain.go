package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"time"
)

func listDir(dirname string) ([]os.FileInfo, []os.FileInfo) {
	var dirInfos, fileInfos []os.FileInfo
	sortInfos, _ := ioutil.ReadDir(dirname)
	for _, arg := range sortInfos {
		if arg.IsDir() {
			dirInfos = append(dirInfos, arg)
		} else {
			fileInfos = append(fileInfos, arg)
		}
	}
	return dirInfos, fileInfos
}

func main() {
	start := time.Now()
	currentDir, _ := os.Getwd()
	fmt.Println(currentDir)
	prevDir := filepath.Dir(currentDir)
	if prevDir == "/" {
		fmt.Println("prevDir - корневая директория")
	} else {
		prevDirInfos, prevFileInfos := listDir(prevDir)
		prevResortInfos := append(prevDirInfos, prevFileInfos...)
		for _, arg := range prevResortInfos {
			fmt.Println(arg.Name())
			break
		}
	}
	fmt.Println(prevDir)
	dirInfos, fileInfos := listDir(currentDir)
	currentCursor := 0 //по умолчанию Up (0 int)
	// prevCursor := 0 //по умолчанию Up
	// nextCursor := 0 //по умолчанию Up
	if len(dirInfos) > 0 {
		fmt.Println(dirInfos[currentCursor].Name()) //выводим содержимое этой директории nextSortInfos
	}
	resortInfos := append(dirInfos, fileInfos...)
	for _, arg := range resortInfos {
		fmt.Println(arg.Name())
	}
	fmt.Printf("%.5f\n", (time.Since(start).Seconds()))
}
