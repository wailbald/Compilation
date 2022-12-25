all: compil

compil:

run_test: test_token
	./test

test_token: src/test_token.cpp src/token.cpp
	g++ $^ -o $@