ELF=main
SRC=$(shell find -name '*.cpp')
OBJECT = $(SRC:.cpp=.o)
$(ELF):$(OBJECT)
	g++ -g -Wall $(OBJECT) -o $(ELF) -lpthread -lrt -lsqlite3
$(OBJECT):
clean:
	rm -f $(OBJECT) $(ELF)


# .PHONY: clean all
# CC = gcc
# CXX = g++
# CPPFLAGS = -Wall -Wno-deprecated
# LDFLAGS = -lpthread -lrt -lsqlite3

# BINSRCS := main.cpp RealTimeLog.cpp
# SRCS := $(wildcard *.cpp)
# SRCS := $(filter-out $(BINSRCS),$(SRCS))

# BIN = main RealTimeLog
# all: $(BIN)

# main::$(SRCS)  main.cpp 
# 	$(CXX) $(CPPFLAGS)  $^ -o $@ $(LDFLAGS)

# RealTimeLog::$(SRCS) RealTimeLog.cpp
# 	$(CXX) $(CPPFLAGS)  $^ -o $@ $(LDFLAGS)

# %.o:%.cpp
# 	$(CXX) $(CPPFLAGS) -c $<
	
# %.o:%.c
# 	$(CC) -c $<
	
# clean:
# 	-rm -rf $(BIN)  *.o