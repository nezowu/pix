package main

/*
#cgo LDFLAGS: -lncursesw -L. -lcurs
#include "my.h"
*/
import "C"

func main() {
	C.goin()
}
