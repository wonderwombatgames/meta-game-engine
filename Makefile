sources=entities_manager.cpp \
				backend_audio_null.cpp \
				backend_physic_null.cpp \
				backend_graphics_sdl.cpp \
				unit_tests.cpp

flags=`pkg-config --cflags --libs sdl2`

all: tests

clean:
	rm -f ./*.x 2> /dev/null

tests: compile_tests run_tests

compile_tests: unit_tests.cpp ${sources} *.hpp
	g++ -std=c++11 -o unit_tests.x ${sources} ${flags}

run_tests:
	./unit_tests.x

sdl_test:
	g++ -std=c++11 -o sdl_sandbox.x sdl_sandbox.cpp ${flags}
	./sdl_sandbox.x
