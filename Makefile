all: clean compile install

compile: clean
	g++ courseditor.cpp -ljsoncpp -o courseditor

execute: 
	./courseditor

clean:
	rm -f *.o *.out main debug courseditor

install-deps-arch:
	yay -S gcc jsoncpp

install:
	cp configs/cours2022.json /home/ay/.config/cours2022.json 
	cp configs/cours2022.log /home/ay/.config/cours2022.log 
	sudo install -m 755 courseditor /usr/bin

# debug:
# 	g++ --std=c++11 -g -o debug main.cpp
# 
# run-debug: debug
# 	ddd debug
# 
# debug-2:
# 	g++ test.cpp -o test.o -ljsoncpp && ./test.o
