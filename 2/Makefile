CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
EXECUTABLE_FILES = StoreCalculator.out Province.out City.out Store.out
OBJ = utils.o

all: $(OBJ) $(EXECUTABLE_FILES)

utils.o : utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp -lboost_filesystem -lboost_system -o $@

StoreCalculator.out : StoreCalculator.cpp utils.cpp
	$(CXX) $(CXXFLAGS) StoreCalculator.cpp utils.o -lboost_filesystem -lboost_system -o $@

Province.out : Province.cpp utils.cpp
	$(CXX) $(CXXFLAGS) Province.cpp utils.o -lboost_filesystem -lboost_system -o $@  

City.out : City.cpp utils.cpp
	$(CXX) $(CXXFLAGS) City.cpp utils.o -lboost_filesystem -lboost_system -o $@  

Store.out : Store.cpp utils.cpp
	$(CXX) $(CXXFLAGS) Store.cpp utils.o -lboost_filesystem -lboost_system -o $@  

.PHONY: clean
clean:
	rm -rf *.out *.o tmp/*