CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -O2 

SRCS = main.cpp permutation.cpp graphinitialisation.cpp colourlists.cpp conflictgraph.cpp greedyalgorithm.cpp output.cpp

TARGET = myprog

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)