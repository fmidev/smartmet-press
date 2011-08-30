HTML = qdpress
PROG = qdpress

# Expected input:
#
# PROG = program name or names
# HTML = name of the group of programs (HTML.dox is deduced)
# CFLAGS
# CC
# LDFLAGS
# ARFLAGS

# Installation directories

ifeq ($(origin PREFIX), undefined)
  prefix = /usr
else
  prefix = $(PREFIX)
endif

processor := $(shell uname -p)
ifeq ($(processor), x86_64)
  libdir = $(prefix)/lib64
else
  libdir = $(prefix)/lib
endif

objdir = obj
includedir = $(prefix)/include

ifeq ($(origin BINDIR), undefined)
  bindir = $(prefix)/bin
else
  bindir = $(BINDIR)
endif

# rpm variables
CWP = $(shell pwd)
CWD = $(shell basename $(CWP))
BIN = $(shell basename $(CWP))
specfile = /smartmet/src/redhat/SPECS/$(CWD).spec
rpmsourcedir = /smartmet/src/redhat/SOURCES
rpmerr = "There's no spec file ($(specfile)). RPM wasn't created. Please make a spec file or copy and rename it into $(specfile)"

DEFINES = -DUNIX -DBOOST_DISABLE_THREADS

MAINFLAGS = -Wall -W -Wno-unused-parameter

EXTRAFLAGS = -Werror \
	-pedantic \
	-Wpointer-arith \
	-Wcast-qual \
	-Wcast-align \
	-Wwrite-strings \
        -Winline \
	-Wctor-dtor-privacy \
        -Wnon-virtual-dtor \
        -Wno-pmf-conversions \
	-Wsign-promo \
        -Wchar-subscripts \
	-Wredundant-decls \
        -Woverloaded-virtual \
	-Wno-long-long

DIFFICULTFLAGS = -Weffc++ -Wunreachable-code -Wold-style-cast  -Wconversion


CC = g++

CFLAGS_DEBUG = -O0 -g $(DEFINES) $(MAINFLAGS) $(EXTRAFLAGS) -Werror
CFLAGS_RELEASE = -O2 -DNDEBUG $(DEFINES) $(MAINFLAGS)

LDFLAGS =
ARFLAGS = -r
INCLUDES = -I$(includedir) \
	-I$(includedir)/smartmet \
	-I$(includedir)/smartmet/newbase

LIBS = -L$(libdir) \
	-lsmartmet_newbase \
	-lz \
	-lbz2 \
	-lboost_system \
	-lboost_regex \
	-lboost_iostreams \
	-lboost_filesystem

# Common library compiling template

# CFLAGS

ifeq ($(MAKECMDGOALS),debug)
  CFLAGS = $(CFLAGS_DEBUG)
else
  CFLAGS = $(CFLAGS_RELEASE)
endif

# Compilation directories

vpath %.cpp source
vpath %.h include
vpath %.o $(objdir)

# How to install

INSTALL_PROG = install -m 775
INSTALL_DATA = install -m 664

# The files to be compiled

SRCS = $(patsubst source/%,%,$(wildcard *.cpp source/*.cpp))
HDRS = $(patsubst include/%,%,$(wildcard *.h include/*.h))
OBJS = $(SRCS:%.cpp=%.o)

OBJFILES = $(OBJS:%.o=obj/%.o)

MAINSRCS = $(PROG:%=%.cpp)
SUBSRCS = $(filter-out $(MAINSRCS),$(SRCS))
SUBOBJS = $(SUBSRCS:%.cpp=%.o)
SUBOBJFILES = $(SUBOBJS:%.o=obj/%.o)

ALLSRCS = $(wildcard source/*.cpp *.cpp )

INCLUDES := -Iinclude $(INCLUDES)

.PHONY: test rpm

# The rules

all: objdir $(PROG)
debug: objdir $(PROG)
release: objdir $(PROG)

$(PROG): % : $(SUBOBJS) %.o
	$(CC) $(LDFLAGS) -o $@ obj/$@.o $(SUBOBJFILES) $(LIBS)

$(LIBFILE): objdir $(OBJS)
	$(AR) $(ARFLAGS) $(LIBFILE) $(OBJFILES)

clean:
	rm -f $(PROG) $(OBJFILES) *~ source/*~ include/*~

install:
	mkdir -p $(bindir)
	@list='$(PROG)'; \
	for prog in $$list; do \
	  echo $(INSTALL_PROG) $$prog $(bindir)/$$prog; \
	  $(INSTALL_PROG) $$prog $(bindir)/$$prog; \
	done

depend:
	gccmakedep -fDependencies -- $(CFLAGS) $(INCLUDES) -- $(ALLSRCS)

test:
	cd test && make test

html::
	mkdir -p ../../../../html/bin/$(HTML)
	doxygen $(HTML).dox

objdir:
	@mkdir -p $(objdir)

rpm: clean
	if [ -e $(BIN).spec ]; \
	then \
	  tar -C ../ -cf $(rpmsourcedir)/smartmet-$(BIN).tar $(BIN) ; \
	  gzip -f $(rpmsourcedir)/smartmet-$(BIN).tar ; \
	  rpmbuild -ta $(rpmsourcedir)/smartmet-$(BIN).tar.gz ; \
	else \
	  echo $(rpmerr); \
	fi;

.SUFFIXES: $(SUFFIXES) .cpp

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(objdir)/$@ $<

-include Dependencies
