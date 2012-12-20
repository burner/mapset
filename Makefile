all: mapset.hpp unit.hpp example.cpp
	g++ example.cpp --std=gnu++11 -o example -Wall -ggdb
	./example
