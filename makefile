#makefile1 适用于一个.cpp与一个.o文件相互对应
SRCS:=${wildcard *.cpp}#将所有cpp文件存储在变量SRCS中
OBJS:=${patsubst %.cpp,%.o,${SRCS}}#将SRCS所有.cpp文件替换为.o文件并存储在OBJS变量中

#makefile2 适用于多个main函数
#eg:
all:1 2 3
1:1.cpp
	g++ 1.cpp -o 1
2:2.cpp
	g++ 2.cpp -o 2
3:3.cpp
	g++ 3.cpp -o 3

#优化
SRCS:=${wildcard *.cpp}
EXEC:=${patsubst %.cpp,%.o,${SRCS}}
all:${EXEC}

%:%.cpp
	g++ $^ -o $@ 
