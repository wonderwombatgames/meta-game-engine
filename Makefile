buildflags=`pkg-config --cflags --libs sdl2`
buildflags+=`pkg-config --cflags --libs SDL2_image`

build:
	rm -f unity.cpp
	g++ -g -std=c++11 -o demo.x *.cpp ${buildflags}

unity:
	$(shell ./unify.sh)
	g++ -g -std=c++11 -o demo.x unity.cpp ${buildflags}

clean:
	rm -f ./*.x 2> /dev/null
	rm -f unity.cpp

format:
	clang-format -i -style=file *.cpp *.hpp

tests: unity
	./demo.x

rebuild: clean
	g++ -g -std=c++11 -o demo.x *.cpp ${buildflags}

all: unity
