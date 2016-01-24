g++flags = -g -std=c++11 -Wall -Weffc++
buildflags=`pkg-config --cflags --libs sdl2`
buildflags+=`pkg-config --cflags --libs SDL2_image`

pch: *.hpp
	g++ ${g++flags} *.hpp

build:
	rm -f unity.cpp
	g++ ${g++flags} -o demo.x *.cpp ${buildflags}

unity:
	$(shell ./unify.sh)
	g++ ${g++flags} -o demo.x unity.cpp ${buildflags}

clean:
	rm -f ./*.x 2> /dev/null
	rm -f unity.cpp

clean_all:
	rm -f ./*.x 2> /dev/null
	rm -f *.gch 2> /dev/null
	rm -f unity.cpp 2> /dev/null

format:
	clang-format -i -style=file *.cpp *.hpp

tests: unity
	./demo.x

rebuild: clean
	g++ ${g++flags} -o demo.x *.cpp ${buildflags}

all: unity
