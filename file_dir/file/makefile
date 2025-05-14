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

dup2:dup2.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ file1
	-rm ./$@

duplex_pipe1:duplex_pipe1.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ pipe1 pipe2
	-rm ./$@

duplex_pipe2:duplex_pipe2.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ pipe2 pipe1
	-rm ./$@

block_replace:block_replace.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ page_data
	-rm ./$@