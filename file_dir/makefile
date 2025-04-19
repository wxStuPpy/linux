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

read:read.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ file1
	-rm ./$@

my_cp:my_cp.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ file1 file2
	-rm ./$@

mmap:mmap.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ file1
	-rm ./$@

lseek:lseek.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ file1
	-rm ./$@

