SRCS:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))

CXX:=g++
CXXFLAG:=-Wall -std=c++11 -lpthread -g

server:$(OBJS)
	$(CXX) $^ -o $@ ${CXXFLAG}

%.o:%.cpp
	$(CXX) -c $^ -o $@ -g

run: server
	./server 0.0.0.0 8888 3

clean:
	rm -f *.o server