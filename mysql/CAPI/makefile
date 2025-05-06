CXX=g++

CXXFLAGS=-Wall  -std=c++11 -lpthread -L/usr/lib64/mysql  -lmysqlclient

mysql_init:mysql_init.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@

mysql_init2:mysql_init2.cpp
	-${CXX} $^ ${CXXFLAGS} -o $@
	-./$@ 
	-rm ./$@


