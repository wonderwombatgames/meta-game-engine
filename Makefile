buildflags=`pkg-config --cflags --libs sdl2`
buildflags+=`pkg-config --cflags --libs SDL2_image`

unity:
	$(shell ./unify.sh)
	g++ -g -std=c++11 -o demo.x unity.cpp ${buildflags}

all: tests

clean:
	rm -f ./*.x 2> /dev/null
	rm unity.cpp

format:
	clang-format -i -style=file *.cpp *.hpp

tests: unity
	./demo.x
