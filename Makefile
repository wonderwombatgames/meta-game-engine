sourcesfiles=\
		entities_manager.cpp \
		entity_base.cpp \
		entity_transformable.cpp \
		backend_audio_null.cpp \
		backend_physics_null.cpp \
		backend_graphic_sdl.cpp \
		backend_handler_sdl.cpp	\
		system_graphics.cpp	\
		system_transform.cpp	\
		utils_colour.cpp	\
		utils_types.cpp	\
		command_router.cpp	\
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
