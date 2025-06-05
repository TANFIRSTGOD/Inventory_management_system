CXX = g++
CC = gcc
CXXFLAGS = -std=c++17 -Wall
CFLAGS = -Wall
LDFLAGS = 

SRC_CPP = main.cpp
SRC_C = sqlite3.c

OBJ_CPP = $(SRC_CPP:.cpp=.o)
OBJ_C = $(SRC_C:.c=.o)

TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ_CPP) $(OBJ_C)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ_CPP) $(OBJ_C)


