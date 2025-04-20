CXX=g++

CXXFLAGS=-Wall  -std=c++11

a:a.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ pipe1 pipe2
	-rm ./$@

b:b.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ pipe1 pipe2
	-rm ./$@