include ../../setup.make

OBJS = FlxTable.o FluxGen.o EvtVector.o DetectorGeom.o EvtRate.o UnionShape.o DiffShape.o Writer.o TimeStruct.o Track.o
INCFILES = FlxTable.h EvtRate.h EvtVector.h FluxGen.h \
		DetectorGeom.h Track.h neutrinos.h Writer.h UnionShape.h DiffShape.h TimeStruct.h

LIBDIR= ../../lib
INCDIR= ../../include/
#CXX= clang++

CXXFLAGS += -Werror -Wall -g

CXXFLAGS +=$(shell root-config --cflags)

INC :=$(addprefix $(INCDIR),$(INCFILES))
LIB :=$(LIBDIR)/libFluxCore.a
.PHONY: clean inc lib

all: $(LIB) $(INC)

inc: $(INC)

lib: $(LIB)

%o::%c 
	$(CXX) $(CXXFLAGS) -c $? -o $@

$(LIB): $(OBJS)
	ar rcs $@ $?


clean:
	rm -f *.o

$(INCDIR)%::%
	cp $^ $@
