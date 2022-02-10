compile:
	g++ main.cpp

execute:
	./a.out

clean:
	rm -f *.o *.out main debug

debug:
	g++ --std=c++11 -g -o debug main.cpp

run-debug: debug
	ddd debug

debug-2:
	g++ test.cpp -o test.o && ./test.o


a: compile execute