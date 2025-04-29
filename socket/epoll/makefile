CXX=g++

CXXFLAGS=-Wall  -std=c++11

epoll_base_server:epoll_base_server.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 192.168.88.128 8888
	-rm ./$@

epoll_base_client:epoll_base_client.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 192.168.88.128 8888
	-rm ./$@


