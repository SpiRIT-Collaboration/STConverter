ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)
UNAME = $(shell uname -rs | cut -d. -f 1)
MAVERICKS = Darwin 13
ifeq ($(UNAME),$(MAVERICKS))
  CXX = g++ -stdlib=libstdc++
else
  CXX = g++
endif

PREFIX = GET
SRCDIR = src
INCDIR = include
TARGET = GETDecoder
LIBDEST = ../..
CONFIG = GETConfig.hh

LIBNAME = lib$(TARGET)
SOURCE = $(wildcard $(SRCDIR)/$(PREFIX)*.cc)
HEADER = $(filter-out %$(CONFIG), $(wildcard $(INCDIR)/$(PREFIX)*.hh))
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.hh
FLAGS = -I./$(INCDIR) $(ROOT_CFLAGS)

$(LIBNAME).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(ROOT_LIBS) $^
	@ln -sf $(shell pwd)/$(LIBNAME).so $(LIBDEST)

$(DICT): $(HEADER) $(LINKDEF)
	rootcint -f $@ -c $^
	$(CXX) -c -fPIC $@ $(FLAGS)

$(LINKDEF):
	@echo "" > LinkdefSpace
	@echo "#ifdef __CINT__" > LinkdefHeader
	@$(shell ls $(INCDIR) | grep ^$(PREFIX) | grep hh | awk -F. {'if ($$1 != "GETConfig") printf("#pragma link C++ class %s+;\n", $$1)'} > LinkdefBody)
	@echo "#endif" > LinkdefFooter
	@cat LinkdefHeader LinkdefSpace LinkdefBody LinkdefSpace LinkdefFooter > $@
	@rm -rf LinkdefSpace LinkdefHeader LinkdefBody LinkdefFooter

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf *.o $(SRCDIR)/*.o
	@rm -rf $(LINKDEF)
	@rm -rf $(LIBNAME).* $(LIBDEST)/$(LIBNAME).so

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(FLAGS) $<
