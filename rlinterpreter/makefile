CC=g++

LIBNAME=rltype
LIBFILE=lib$(LIBNAME).a
BUILDDIR=build

INCLUDE=-I.

SRC = RLInterpreter.cpp RLType.cpp RLCommand.cpp

OBJ = $(SRC:.cpp=.o)

.SUFFIXES: .cpp

.cpp.o:
	$(CC) $(DEBUG) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(LIBNAME): $(OBJ)
	ar rc $(LIBFILE) $(OBJ)

depend: dep

dep:
	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f $(OBJ) $(LIBFILE)
