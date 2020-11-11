CC:=gcc
CXX:=g++
LD:=ld

CFLAGS+= -g -Wall

CXXFLAGS+= -g -Wall
CXXFLAGS+= -std=c++11

INCLUDES+= -I./include

LDLIBS+= -pthread

OBJS:= llcan.o \

TARGETS:= cansnd canrcv

all: $(TARGETS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@


cansnd: $(OBJS) cansnd.o
	$(CXX) $(LDLIBS) $^ -o $@

canrcv: $(OBJS) canrcv.o
	$(CXX) $(LDLIBS) $^ -o $@

clean:
	rm -rf $(OBJS) *.o $(TARGETS)
