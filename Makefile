LIB = press

CC = g++
CFLAGS = -DUNIX -O0 -g -Wall
CFLAGS_RELEASE = -DUNIX -O2 -DNDEBUG -Wall
LDFLAGS = -s
ARFLAGS = -r
INCLUDES = -I $(includedir)/newbase

# Common library compiling template

include ../../makefiles/makefile.lib
