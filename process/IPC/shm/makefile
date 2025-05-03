CXX:=g++

CXXFLAGS:=-Wall  -std=c++11

shmget:shmget.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@

