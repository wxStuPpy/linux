CXX=g++

CXXFLAGS=-Wall  -std=c++11

server1:server1.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 192.168.88.110 8888
	-rm ./$@

client1:client1.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 192.168.88.110 8888
	-rm ./$@


server2:server2.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 192.168.88.110 8888
	-rm ./$@

client2:client2.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 192.168.88.110 8888
	-rm ./$@


server3:server3.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 192.168.88.110 8888
	-rm ./$@

client3:client3.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 192.168.88.110 8888
	-rm ./$@