CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean
all: parent child

parent: src/parent.cpp src/common.h
	$(CXX) $(CXXFLAGS) src/parent.cpp -o parent

child: src/child.cpp src/common.h
	$(CXX) $(CXXFLAGS) src/child.cpp -o child

clean:
	rm -f parent child
