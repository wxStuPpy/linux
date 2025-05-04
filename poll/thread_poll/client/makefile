CXX=g++

CXXFLAGS=-Wall  -std=c++11 -lpthread

client:client.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 0.0.0.0 8888
	-rm ./$@