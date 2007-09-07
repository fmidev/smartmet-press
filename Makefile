LIB = press

MAINFLAGS = -Wall -W -Wno-unused-parameter

EXTRAFLAGS = -Werror -pedantic -Wpointer-arith -Wcast-qual \
	-Wcast-align -Wwrite-strings -Wconversion -Winline \
	-Wctor-dtor-privacy -Wnon-virtual-dtor -Wno-pmf-conversions \
	-Wsign-promo -Wchar-subscripts -Wold-style-cast \
	-Wredundant-decls -Wshadow

DIFFICULTFLAGS = -Weffc++l -Wunreachable-code -Woverloaded-virtual

CC = g++
CFLAGS = -DUNIX -O0 -g $(MAINFLAGS) $(EXTRAFLAGS) -Werror
CFLAGS_RELEASE = -DUNIX -O2 -DNDEBUG $(MAINFLAGS)
LDFLAGS = -s
ARFLAGS = -r
INCLUDES = -I $(includedir)/smartmet/newbase
LIBS = -L $(libdir) -lsmartmet-newbase

# Common library compiling template

# Installation directories

prosessor := $(shell uname -p)

ifeq ($(origin PREFIX), undefined)
  PREFIX = /usr
else
  PREFIX = $(PREFIX)
endif

ifeq ($(prosessor), x86_64)
  libdir = $(PREFIX)/lib64
else
  libdir = $(PREFIX)/lib
endif

bindir = $(PREFIX)/bin
includedir = $(PREFIX)/include
objdir = obj

# rpm variables
rpmsourcedir = /smartmet/src/redhat/SOURCES
rpmerr = "There's no spec file ($(LIB).spec). RPM wasn't created. Please make a spec file or copy and rename it into $(LIB).spec"

# What to install

LIBFILE = libsmartmet_$(LIB).a
PGLIBFILE = libsmartmet_$(LIB)pg.a

# How to install

INSTALL_PROG = install -m 775
INSTALL_DATA = install -m 664

# CFLAGS

ifneq (,$(findstring release,$(MAKECMDGOALS)))
  CFLAGS = $(CFLAGS_RELEASE)
endif

ifneq (,$(findstring gmon,$(MAKECMDGOALS)))
 objdir := pgobj
 CFLAGS := -pg $(CFLAGS)
endif

# Compilation directories

vpath %.cpp source
vpath %.h include
vpath %.o $(objdir)

# The files to be compiled

SRCS = $(patsubst source/%,%,$(wildcard source/*.cpp))
HDRS = $(patsubst include/%,%,$(wildcard include/*.h))
OBJS = $(SRCS:%.cpp=%.o)

OBJFILES = $(OBJS:%.o=obj/%.o)
PGOBJFILES = $(OBJS:%.o=pgobj/%.o)

INCLUDES := -I include $(INCLUDES)

.PHONY: test gmon rpm

# The rules

all: objdir $(LIBFILE)
debug: all
release: all

$(LIBFILE): $(OBJS)
	$(AR) $(ARFLAGS) $(LIBFILE) $(OBJFILES)

$(PGLIBFILE): $(OBJS)
	$(AR) $(ARFLAGS) $(PGLIBFILE) $(PGOBJFILES)

clean:
	rm -f $(LIBFILE) $(OBJFILES) $(PGOBJFILES) *~ source/*~ include/*~

install:
	@mkdir -p $(includedir)/$(LIB)
	@list='$(HDRS)'; \
	for hdr in $$list; do \
	  if [[ include/$$hdr -nt $(includedir)/$(LIB)/$$hdr ]]; \
	  then \
	    echo $(INSTALL_DATA) include/$$hdr $(includedir)/$(LIB)/$$hdr; \
	  fi; \
	  $(INSTALL_DATA) include/$$hdr $(includedir)/$(LIB)/$$hdr; \
	done
	@mkdir -p $(libdir)
	$(INSTALL_DATA) $(LIBFILE) $(libdir)/$(LIBFILE)

depend:
	makedepend $(INCLUDES)

test:
	cd test && make test

html::
	mkdir -p /data/local/html/lib/$(LIB)
	doxygen $(LIB).dox

objdir:
	@mkdir -p $(objdir)

rpm: clean depend
	if [ -a $(LIB).spec ]; \
	then \
	  tar -C ../ -cf $(rpmsourcedir)/libsmartmet-$(LIB).tar $(LIB) ; \
	  gzip -f $(rpmsourcedir)/libsmartmet-$(LIB).tar ; \
	  rpmbuild -ta $(rpmsourcedir)/libsmartmet-$(LIB).tar.gz ; \
	else \
	  echo $(rpmerr); \
	fi;

gmon: objdir $(PGLIBFILE) gmon.txt

gmon.txt: $(PGLIBFILE)
	@echo Creating a temporary profiling program
	@echo "int main(int argc, char ** argv){}" > foobar.cpp
	@g++ -pg -o foobar foobar.cpp -Wl,-whole-archive -L. -l$(LIB)pg -Wl,-no-whole-archive $(LIBS)
	@./foobar
	@rm -f foobar.cpp
	@echo Created temporary gmon.out
	@gprof -b -q -c -z foobar gmon.out > gmon.txt
	@rm -f foobar gmon.out
	@echo Created gmon.txt for analysis

headertest:
	@echo "Checking self-sufficiency of each header:"
	@echo
	@for hdr in $(HDRS); do \
	echo $$hdr; \
	echo "#include \"$$hdr\"" > /tmp/$(LIB).cpp; \
	echo "int main() { return 0; }" >> /tmp/$(LIB).cpp; \
	$(CC) $(CFLAGS) $(INCLUDES) -o /dev/null /tmp/$(LIB).cpp $(LIBS); \
	done

.SUFFIXES: $(SUFFIXES) .cpp

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $(objdir)/$@ $<

EXTRAS = $(wildcard docs/*.doc docs/*.html docs/*.gif docs/*xls)
HTML = ../../../../html/lib/$(LIB)

html::
	mkdir -p $(HTML)/docs
	@list='$(EXTRAS)'; \
	for name in $$list; do \
	  echo $(INSTALL_DATA) $$name $(HTML)/$$name; \
	  $(INSTALL_DATA) $$name $(HTML)/$$name; \
	done

# -include Dependencies
# DO NOT DELETE THIS LINE -- make depend depends on it.

