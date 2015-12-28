all: tests

clean:
	rm -f ./*.x 2> /dev/null

tests: compile_tests run_tests

compile_tests: entities_manager.cpp unit_tests.cpp *.hpp
	g++ -std=c++11 -o unit_tests.x entities_manager.cpp unit_tests.cpp

run_tests:
	./unit_tests.x

sdl_test:
	g++ -std=c++11 -o sdl_sandbox.x sdl_sandbox.cpp `pkg-config --cflags --libs sdl2`
	./sdl_sandbox.x
