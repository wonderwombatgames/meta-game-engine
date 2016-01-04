sourcesfiles=entities_manager.cpp \
		backend_audio_null.cpp \
		backend_physic_null.cpp \
		backend_graphic_sdl.cpp \
		context_sdl.cpp	\
		utils_colour.cpp \
		command_router.cpp \
		unit_tests.cpp

buildflags=`pkg-config --cflags --libs sdl2`
buildflags+=`pkg-config --cflags --libs SDL2_image`


all: tests

build: unit_tests.cpp ${sourcesfiles} *.hpp
	g++ -g -std=c++11 -o unit_tests.x ${sourcesfiles} ${buildflags}

clean:
	rm -f ./*.x 2> /dev/null

tests: build
	./unit_tests.x
