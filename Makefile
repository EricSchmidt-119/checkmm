checkmm: checkmm.cpp
	g++ -O2 -Wall -pedantic -std=c++20 checkmm.cpp -o checkmm

checkmm-debug: checkmm.cpp
	g++ -g -Wall -pedantic -std=c++20 checkmm.cpp -o checkmm-debug

test: checkmm
	cd tests && ./runtests

.PHONY: test
