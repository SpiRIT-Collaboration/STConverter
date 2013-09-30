ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)
CXX = g++

PREFIX = ST
SRCDIR = src
INCDIR = include

GETDecoderINCDIR = ../GETDecoder/include
GETDecoderLIB = ../libGETDecoder.so

LIBNAME = libSPiRIT
SOURCE = $(wildcard $(SRCDIR)/$(PREFIX)*.cc)
HEADER = $(wildcard $(INCDIR)/$(PREFIX)*.hh)
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.hh
FLAGS = -I./$(INCDIR) -I$(GETDecoderINCDIR) $(ROOT_CFLAGS)
LIBDEST = ../..

$(LIBNAME).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(ROOT_LIBS) $^ $(GETDecoderLIB)
	@ln -sf $(shell pwd)/$(LIBNAME).so $(LIBDEST)

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

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(FLAGS) $<
