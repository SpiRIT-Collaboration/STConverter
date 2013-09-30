ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)
CXX = g++

LIBNAME = libGETDecoder
SOURCE = $(wildcard GET*.cc)
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.hh
LIBDEST = ..

$(LIBNAME).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(ROOT_LIBS) $^
	@ln -sf $(shell pwd)/$(LIBNAME).so $(LIBDEST)

$(DICT): $(SOURCE:.cc=.hh) $(LINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(ROOT_CFLAGS)

$(LINKDEF):
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader
	@$(shell ls | grep ^GET | grep cc | awk -F. {'printf("#pragma link C++ class %s+;\n", $$1)'} > LinkdefBody)
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader LinkdefSpace LinkdefBody LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader LinkdefBody LinkdefFooter

clean:
	@rm -rf $(LIBNAME)Dict.cc
	@rm -rf *.o
	@rm -rf $(LIBNAME).so
	@rm -rf $(MAKEHITS)
	@rm -rf $(DICT:.cc=.*)
	@rm -rf $(LINKDEF)
	@rm -rf $(LIBDEST)/$(LIBNAME).so

%.o: %.cc
	$(CXX) -c -fPIC $(ROOT_CFLAGS) $<
