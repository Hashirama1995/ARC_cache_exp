IDIR =../include
CC=g++
CFLAGS=-g -I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = RpARCache.h RpList.h RpMap.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = RpARCache.o RpList.o RpMap.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ARC: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
