checkmm: checkmm.cpp
	g++ -O2 -Wall -pedantic -std=c++20 checkmm.cpp -o checkmm

test: checkmm
	cd tests && ./runtests

.PHONY: test
