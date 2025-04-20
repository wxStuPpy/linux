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

b_kick:b_kick.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ pipe1 pipe2
	-rm ./$@

nonBlock:nonBlock.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ pipe1 
	-rm ./$@