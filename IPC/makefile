CXX:=g++

CXXFLAGS:=-Wall  -std=c++11

popen:popen.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@

pipe_both_side:pipe_both_side.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@