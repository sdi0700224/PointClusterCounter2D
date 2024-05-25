CXX = g++
CXXFLAGS = -std=c++17 -fopenmp -O2 -Wall -Werror
LDFLAGS = -fopenmp

INCLUDES = -Iinclude
SRCS = src/UnionFind.cpp src/ClusterCounter.cpp src/main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = clusterCounter

ARGS = 1000000 1.5

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(EXEC)
	./$(EXEC) $(ARGS)

debug: $(EXEC)
	valgrind ./$(EXEC) $(ARGS)
	ps
	ipcs

clean:
	rm -f $(OBJS) $(EXEC)
