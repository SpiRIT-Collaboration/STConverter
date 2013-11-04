ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)
CXX = g++

PREFIX = ST
SRCDIR = src
INCDIR = include

GETDecoderDIR = $(shell pwd)/../GETDecoder
GETDecoderINCDIR = $(GETDecoderDIR)/include
#GETDecoderLIB = ../GETDecoder/libGETDecoder.so

LIBNAME = libSPiRIT
SOURCE = $(wildcard $(SRCDIR)/$(PREFIX)*.cc)
HEADER = $(wildcard $(INCDIR)/$(PREFIX)*.hh)
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.hh
FLAGS = -I./$(INCDIR) -I$(GETDecoderINCDIR)  $(ROOT_CFLAGS)
LIBDEST = ../..

$(LIBNAME).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(ROOT_LIBS) $^ -L$(GETDecoderDIR) -lGETDecoder
#	$(CXX) --shared -fPIC -o $@ $(ROOT_LIBS) $^ $(GETDecoderDIR)/libGETDecoderDict.o $(GETDecoderDIR)/src/GETDecoder.o $(GETDecoderDIR)/src/GETFrame.o 
	@ln -sf $(shell pwd)/$(LIBNAME).so $(LIBDEST)
	@ln -sf $(shell pwd)/mapping/ChToPad.map $(LIBDEST)
	@ln -sf $(shell pwd)/mapping/UnitAsAd.map $(LIBDEST)

$(DICT): $(HEADER) $(LINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(FLAGS)

$(LINKDEF):
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader
	@$(shell ls $(INCDIR) | grep ^$(PREFIX) | grep hh | awk -F. {'printf("#pragma link C++ class %s+;\n", $$1)'} > LinkdefBody)
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader LinkdefSpace LinkdefBody LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader LinkdefBody LinkdefFooter

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf *.o $(SRCDIR)/*.o
	@rm -rf $(LINKDEF)
	@rm -rf $(LIBNAME).so $(LIBDEST)/$(LIBNAME).so
	@rm -rf $(LIBDEST)/*.map

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(FLAGS) $<
