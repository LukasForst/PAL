CPPFLAGS+= -std=c++11 -pipe -Wall -O3

main: main.o
	${CXX} ${CPPFLAGS} main.o -o main

main.o: main.cpp
	${CXX} ${CPPFLAGS} -o main.o -c main.cpp

run: main
	./main

zip:
	zip hw_forst.zip *.cpp

clean:
	rm -f *.out
	rm -f *.o
	rm -f main
	rm -f *.zip
