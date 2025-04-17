CXX=g++

CXXFLAGS=-Wall  -std=c++11

my_ls:my_ls.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ dir1
	-rm ./$@
my_lsal:my_ls_al.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ ../dir_stream
	-rm ./$@

tree:tree.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ ../dir_stream
	-rm ./$@