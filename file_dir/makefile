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

getcwd:getcwd.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ file1
	-rm ./$@

open:open.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ file2
	-rm ./$@
