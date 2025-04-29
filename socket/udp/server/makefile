CXX=g++

CXXFLAGS=-Wall  -std=c++11

server1:server1.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 192.168.88.110 8888
	-rm ./$@
