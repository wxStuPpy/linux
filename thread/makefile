CXX:=g++

CXXFLAGS:=-Wall  -std=c++11 -lpthread

pthread_create:pthread_create.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@

pthread_heap:pthread_heap.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@

