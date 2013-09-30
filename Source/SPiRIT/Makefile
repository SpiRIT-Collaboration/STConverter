ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)
CXX = g++

LIBNAME = libSPiRIT
MAKEPEDESTAL = makepedestal
SOURCE = $(wildcard ST*.cc)
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.hh

$(LIBNAME).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(ROOT_LIBS) $^

$(DICT): $(SOURCE:.cc=.hh) $(LINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(ROOT_CFLAGS)

$(MAKEPEDESTAL): $(MAKEPEDESTAL).cc
	$(CXX) -o $@ $^ $(ROOT_CFLAGS) $(ROOT_LIBS)

$(LINKDEF):
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader
	@$(shell ls | grep ^ST | grep cc | awk -F. {'printf("#pragma link C++ class %s+;\n", $$1)'} > LinkdefBody)
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader LinkdefSpace LinkdefBody LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader LinkdefBody LinkdefFooter

clean:
	@rm -rf $(LIBNAME)Dict.cc
	@rm -rf *.o
	@rm -rf $(LIBNAME).so
	@rm -rf $(MAKEPEDESTAL)
	@rm -rf $(MAKEHITS)
	@rm -rf $(DICT:.cc=.*)
	@rm -rf $(LINKDEF)

%.o: %.cc
	$(CXX) -c -fPIC $(ROOT_CFLAGS) $<
