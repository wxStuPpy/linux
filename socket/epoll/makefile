CXX=g++

CXXFLAGS=-Wall  -std=c++11

epoll_base_server:epoll_base_server.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 0.0.0.0 8888
	-rm ./$@

epoll_base_client:epoll_base_client.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 0.0.0.0 8888
	-rm ./$@

epoll_server1:epoll_server1.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 0.0.0.0 8888
	-rm ./$@

epoll_client1:epoll_client1.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 0.0.0.0 8888
	-rm ./$@


