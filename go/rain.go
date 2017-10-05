package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"time"
)

func listDir(dirName string) []os.FileInfo {
	var dirInfos, fileInfos []os.FileInfo
	infos, _ := ioutil.ReadDir(dirName)
	for _, arg := range infos {
		if arg.IsDir() {
			dirInfos = append(dirInfos, arg)
		} else {
			fileInfos = append(fileInfos, arg)
		}
	}
	return append(dirInfos, fileInfos...) //, len(dirInfos)
}

func listPrev(dirName string) []os.FileInfo {
	if dirName == "/" {
		var prevInfos []os.FileInfo
		dir, _ := os.Lstat(dirName)
		return append(prevInfos, dir)
	}
	return listDir(filepath.Dir(dirName))
}

func listNext(info os.FileInfo) []os.FileInfo {
	if info.IsDir() {
		pathDir, _ := filepath.Abs(info.Name())
		nextInfos := listDir(pathDir)
		if len(nextInfos) > 0 {
			return nextInfos
		}
	}
	return nil
}

func listTwo() ([]os.FileInfo, []os.FileInfo) {
	currentDir, _ := os.Getwd()
	infos := listDir(currentDir)
	prevInfos := listPrev(currentDir)
	return prevInfos, infos
}

func main() {
	start := time.Now()
	prevInfos, infos := listTwo()
	nextInfos := listNext(infos[0])
	fmt.Println(prevInfos[0].Name(), infos[0].Name(), nextInfos[0].Name())
	//var currentCursor int //0 по умолчанию Up (0 int)
	// var prevCursor int //0 по умолчанию Up
	// var nextCursor int //по умолчанию Up
	fmt.Printf("%.5f\n", (time.Since(start).Seconds()))
}
