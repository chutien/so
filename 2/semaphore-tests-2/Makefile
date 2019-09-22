#
# Makefile to build semaphore.asm
#
# @Piotr Styczynski 2018
# MIT LICENSED
#

./build/proberen_time_test: ./build/proberen_time_test.o ./build/semaphore.o ./build/get_os_time.o
	g++ -lpthread -o ./build/proberen_time_test ./build/semaphore.o ./build/proberen_time_test.o ./build/get_os_time.o

./build/proberen_time_test.o: ./src/proberen_time_test.cpp
	g++ ./src/proberen_time_test.cpp -o ./build/proberen_time_test.o -c

./build/dafuq: ./build/dafuq.o ./build/semaphore.o ./build/get_os_time.o
	g++ -lpthread -o ./build/dafuq ./build/semaphore.o ./build/dafuq.o ./build/get_os_time.o

./build/dafuq.o: ./src/dafuq.cpp
	g++ ./src/dafuq.cpp -o ./build/dafuq.o -c

./build/multi: ./build/multi.o ./build/semaphore.o ./build/get_os_time.o
	g++ -lpthread -o ./build/multi ./build/semaphore.o ./build/multi.o ./build/get_os_time.o

./build/multi.o: ./src/multi.cpp
	g++ ./src/multi.cpp -o ./build/multi.o -c

./build/two: ./build/two.o ./build/semaphore.o ./build/get_os_time.o
	g++ -lpthread -o ./build/two ./build/semaphore.o ./build/two.o ./build/get_os_time.o

./build/two.o: ./src/two.cpp
	g++ ./src/two.cpp -o ./build/two.o -c

./build/simple: ./build/simple.o ./build/semaphore.o ./build/get_os_time.o
	g++ -o ./build/simple ./build/semaphore.o ./build/simple.o ./build/get_os_time.o

./build/simple.o: ./src/simple.cpp
	g++ ./src/simple.cpp -o ./build/simple.o -c

./build/hanging: ./build/hanging.o ./build/semaphore.o ./build/get_os_time.o
	g++ -o ./build/hanging ./build/semaphore.o ./build/hanging.o ./build/get_os_time.o

./build/hanging.o: ./src/hanging.cpp
	g++ ./src/hanging.cpp -o ./build/hanging.o -c

./build/semaphore.o: ./src/semaphore.asm
	nasm -g -F dwarf -f elf64 -o ./build/semaphore.o ./src/semaphore.asm

./build/get_os_time.o: ./src/get_os_time.cpp
	g++ ./src/get_os_time.cpp -o ./build/get_os_time.o -c

clean:
	-rm -f ./build/dafuq ./build/proberen_time_test ./build/proberen_time_test.o ./build/dafuq.o ./build/two ./build/two.o ./build/hanging ./build/hanging.o ./build/simple ./build/simple.o ./build/semaphore.o

all: ./build/dafuq ./build/multi ./build/simple ./build/hanging ./build/two ./build/proberen_time_test ./build/get_os_time.o  ./build/multi  ./build/multi.o