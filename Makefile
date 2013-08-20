ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)
CXX = g++

LIBNAME = libSPiRIT
MAKEPEDESTAL = makepedestal
SOURCE = SPiRITReadRaw.cc SPiRITMap.cc SPiRITPedestal.cc SPiRITEvent.cc SPiRITPad.cc
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.hh

$(LIBNAME).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(ROOT_LIBS) $^

$(DICT): $(SOURCE:.cc=.hh)
	rootcint -f $@ -c $^ $(LINKDEF)
	$(CXX) -c -fPIC $@ $(ROOT_CFLAGS)

$(MAKEPEDESTAL): $(MAKEPEDESTAL).cc
	$(CXX) -o $@ $^ $(ROOT_CFLAGS) $(ROOT_LIBS)

clean:
	@rm -rf $(LIBNAME)Dict.cc
	@rm -rf *.o
	@rm -rf $(LIBNAME).so
	@rm -rf $(CONVERTER)
	@rm -rf $(MAKEHITS)
	@rm -rf $(DICT:.cc=.*)

%.o: %.cc
	$(CXX) -c -fPIC $(ROOT_CFLAGS) $<
