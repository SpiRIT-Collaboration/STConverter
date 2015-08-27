ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)
CXX = g++

PREFIX = ST
SRCDIR = src
INCDIR = include

GETDECODER = ../GETDecoder

LIBNAME = STCore
TARGET = lib$(LIBNAME)
SOURCE = $(wildcard $(SRCDIR)/$(PREFIX)*.cc)
HEADER = $(wildcard $(INCDIR)/$(PREFIX)*.hh)
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.h
INCFLAG = -I./$(INCDIR)/ -I$(GETDECODER)/include 
FLAGS = $(ROOT_CFLAGS) $(INCFLAG)
LIBS = $(ROOT_LIBS) -L$(GETDECODER) -lGETDecoder

$(TARGET).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(LIBS) $^

$(DICT): $(HEADER)
	rootcint -f $@ -c $(INCFLAG) $^ $(LINKDEF)
	$(CXX) -c -fPIC $@ $(FLAGS)

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf $(SRCDIR)/*.o
	@rm -rf $(TARGET).so
	@rm -rf *.pcm

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(FLAGS) $<
