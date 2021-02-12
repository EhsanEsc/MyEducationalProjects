CC = g++
CXXFLAGS = -std=c++11 -Wextra
# LINK_FLAGS = -I $(INCLUDE_DIR) -I $(INLINE_DIR) -pthread
LINK_FLAGS = -pthread

all: PhonePricePrediction.out

PhonePricePrediction.out: main.o
	$(CC) $(CXXFLAGS) $(LINK_FLAGS) main.o -o PhonePricePrediction.out

main.o: main.cpp
	$(CC) $(CXXFLAGS) $(LINK_FLAGS) -c main.cpp -o main.o

clean:
	rm -rf *.o *.out