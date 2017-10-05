test: test.go my.h libcurs.a
	go build test.go

libcurs.a: curs.o
	ar rcs libcurs.a curs.o

curs.o: curs.c
	gcc -c $<
