# The files to be compiled

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=%.o)
PROGS = $(SRCS:%.cpp=%)

MAINFLAGS = -Wall -W -Wno-unused-parameter

EXTRAFLAGS = -Werror -pedantic -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -Wconversion -Winline \
	-Wctor-dtor-privacy -Wnon-virtual-dtor -Wno-pmf-conversions \
	-Wsign-promo -Wchar-subscripts -Wold-style-cast -Wunreachable-code

DIFFICULTFLAGS = -Weffc++ -Wredundant-decls -Wshadow -Woverloaded-virtual

CC = g++
CFLAGS = -DUNIX -O0 -g $(MAINFLAGS) $(EXTRAFLAGS) -Werror
INCLUDES = -I ../../newbase/include -I ../include
LIBS =  -L .. -lpress -L ../../newbase -lnewbase

# The rules

all: $(PROGS)

$(PROGS): % : %.cpp
	$(CC) $(CFLAGS) -o $@ $@.cpp $(INCLUDES) $(LIBS)

clean:
	rm -f $(PROGS) $(OBJS) *~
