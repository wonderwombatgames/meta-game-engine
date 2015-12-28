all: tests

clean: clean_tests clean_sdl

tests: compile_tests run_tests

compile_tests: entities_manager.cpp unit_tests.cpp *.hpp
	g++ -std=c++11 -o unit_tests entities_manager.cpp unit_tests.cpp

run_tests:
	./unit_tests

clean_tests:
	rm ./unit_tests

sdl_test:
	g++ -std=c++11 -o sdl_sandbox sdl_sandbox.cpp `pkg-config --cflags --libs sdl2`
	./sdl_sandbox

clean_sdl:
	rm ./sdl_sandbox
