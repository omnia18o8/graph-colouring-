CXX = g++-15
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O3 

SRCS = main.cpp apst.cpp graphinitialisation.cpp permutation.cpp colourlists.cpp conflictgraph.cpp greedyalgorithm.cpp

TARGET = myprog

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
