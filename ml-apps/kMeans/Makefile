# MAKEFILE

CXX = arm-linux-gnueabi-g++
CFLAGS=-c -O3 -std=c++11 -static
LDFLAGS=-lm -lpthread

SRC=kMeans.cpp 
OBJ=$(SRC:.cpp=.o)
EXE=kMeans

DEPS=$(SRC:%.cpp=%.d)

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) -static $(OBJ) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -MMD -MP $< -o $@

clean:
	rm -rf *.o *.d $(EXE)

-include $(DEPS)
