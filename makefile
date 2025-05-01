all: main

main: main.cpp Car.hpp Lift.hpp Factory.hpp Draw.hpp Kolejka.hpp
	g++ main.cpp -lncurses -o main.out