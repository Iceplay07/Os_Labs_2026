CXX ?= c++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2

all: parent child

parent: parent.cpp
	$(CXX) $(CXXFLAGS) parent.cpp -o parent

child: child.cpp
	$(CXX) $(CXXFLAGS) child.cpp -o child

clean:
	rm -f parent child
