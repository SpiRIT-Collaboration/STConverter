ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs) -lSpectrum
CXX = g++

PREFIX = ST

STCORE = ../STCore
STDATA = ../STData
GETDECODER = ../../../GETDecoder

LIBNAME = STUtil
TARGET = lib$(LIBNAME)
SOURCE = $(wildcard $(PREFIX)*.cc)
HEADER = $(wildcard $(PREFIX)*.hh) #$(wildcard $(STDATA)/$(PREFIX)*.hh) $(wildcard $(STCORE)/$(PREFIX)*.hh) $(wildcard $(GETDECODER)/include/*.hh)
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.h
INCDIR = -I$(STDATA) -I$(STCORE) -I$(GETDECODER)/include
FLAGS = $(ROOT_CFLAGS) $(INCDIR)
LIBS = $(ROOT_LIBS) -L$(STDATA) -lSTData -L$(STCORE) -lSTCore -L$(GETDECODER) -lGETDecoder
LIBDEST = ../../

$(TARGET).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(LIBS) $^ 
	@mkdir -p $(LIBDEST)
	@ln -sf $(shell pwd)/$(TARGET).so $(LIBDEST)

$(DICT): $(HEADER)
	rootcint -f $@ -c $(INCDIR) $^ $(LINKDEF)
	$(CXX) -c -fPIC $@ $(FLAGS)

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf *.o
	@rm -rf $(TARGET).so $(LIBDEST)/$(TARGET).so

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(FLAGS) $<
