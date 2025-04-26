CXX:=g++

CXXFLAGS:=-Wall  -std=c++11 -lpthread

mutex_init:mutex_init.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@

sync:sync.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@


