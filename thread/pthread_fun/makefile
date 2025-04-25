CXX:=g++

CXXFLAGS:=-Wall  -std=c++11 -lpthread

pthread_exit:pthread_exit.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@

pthread_join:pthread_join.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@

pthread_cancel:pthread_cancel.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@


