LIB = press

#MAINFLAGS = -Wall -W -Wno-unused-parameter -march=i586
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

include ../../makefiles/makefile.lib

EXTRAS = $(wildcard docs/*.doc docs/*.html docs/*.gif docs/*xls)
HTML = ../../../../html/lib/$(LIB)

html::
	mkdir -p $(HTML)/docs
	@list='$(EXTRAS)'; \
	for name in $$list; do \
	  echo $(INSTALL_DATA) $$name $(HTML)/$$name; \
	  $(INSTALL_DATA) $$name $(HTML)/$$name; \
	done
