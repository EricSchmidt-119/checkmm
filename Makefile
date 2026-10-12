checkmm: checkmm.cpp
	g++ -O2 -Wall -pedantic -std=c++20 checkmm.cpp -o checkmm

checkmm-debug: checkmm.cpp
	g++ -g -Wall -pedantic -fsanitize=address -std=c++20 checkmm.cpp -o checkmm-debug

test: checkmm checkmm-debug
	cd tests && ./runtests checkmm-debug && ./runtests checkmm

.PHONY: test
