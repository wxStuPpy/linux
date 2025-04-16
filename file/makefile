CXX=g++

CXXFLAGS=-Wall  -std=c++11

fopen:fopen.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ file1
	-rm ./$@
	
chmod:chmod.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ file1
	-rm ./$@
