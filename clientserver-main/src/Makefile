# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of GNU's libstdc++.
# -g is for debugging.
CPPFLAGS =  -I/include
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++11 
CXXFLAGS += -g
CXXFLAGS += $(DEPFLAGS)
LDFLAGS =   -g -Llib
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++

# Targets

all: lib/libclientserver.a
	make -C example


CommandHandler.o: CommandHandler.cc CommandHandler.h
	g++ -c CommandHandler.cc CommandHandler.h include/protocol.h include/connection.h include/connectionclosedexception.h include/server.h -std=c++17

CHtest.o: CHtest.cc 
	g++ -c CHtest.cc include/protocol.h include/connection.h include/connectionclosedexception.h include/server.h -std=c++17

CHtest: CHtest.o CommandHandler.o
	g++ -o CHtest CHtest.o CommandHandler.o -std=c++17

# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

lib/libclientserver.a: src/connection.o src/server.o
	mkdir -p lib
	ar rv lib/libclientserver.a  src/connection.o src/server.o
	ranlib lib/libclientserver.a

# Phony targets
.PHONY: all clean distclean

SRC = $(wildcard src/*.cc)

# Standard clean
clean:
	-rm $(SRC:.cc=.o) $(PROGS)

distclean: clean
	-rm lib/libclientserver.a
	-rmdir lib
	-rm $(SRC:.cc=.d) 
	make -C example distclean


# Include the *.d files
-include $(SRC:.cc=.d)
