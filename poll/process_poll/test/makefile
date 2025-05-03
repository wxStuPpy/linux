CXX = g++
CXXFLAGS = -Wall -std=c++11 -lpthread

# 列出所有源文件
SRCS = test.cpp worker.cpp
# 生成对应的目标文件列表
OBJS = $(SRCS:.cpp=.o)

# 可执行文件
TARGET = test

# 默认目标
all: $(TARGET)

# 链接目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(CXXFLAGS)

# 编译每个源文件生成对应的目标文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 运行可执行文件
run: $(TARGET)
	./$(TARGET)

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)
    