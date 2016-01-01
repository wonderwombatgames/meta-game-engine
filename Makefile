sourcesfiles=entities_manager.cpp \
		backend_audio_null.cpp \
		backend_physic_null.cpp \
		backend_graphics_sdl.cpp \
		sdl_context.cpp	\
		unit_tests.cpp

buildflags=`pkg-config --cflags --libs sdl2`

all: tests #sdl

build: unit_tests.cpp ${sourcesfiles} *.hpp
	g++ -std=c++11 -o unit_tests.x ${sourcesfiles} ${buildflags}

clean:
	rm -f ./*.x 2> /dev/null

tests: build
	./unit_tests.x

sdl:
	g++ -std=c++11 -o sdl_sandbox.x sdl_sandbox.cpp ${buildflags}
	./sdl_sandbox.x
