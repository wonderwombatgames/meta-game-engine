all: tests

clean: clean_tests

tests: compile_tests run_tests

compile_tests: entities_manager.cpp unit_tests.cpp *.hpp
	g++ -std=c++11 -o unit_tests entities_manager.cpp unit_tests.cpp

run_tests:
	./unit_tests

clean_tests:
	rm ./unit_tests
