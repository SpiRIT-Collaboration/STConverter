ROOT_CFLAGS = $(shell root-config --cflags)
ROOT_LIBS = $(shell root-config --libs)
CXX = g++

PREFIX = ST

LIBNAME = STData
TARGET = lib$(LIBNAME)
SOURCE = $(wildcard $(PREFIX)*.cc)
HEADER = $(wildcard $(PREFIX)*.hh)
DICT = $(LIBNAME)Dict.cc
LINKDEF = $(LIBNAME)LinkDef.h
FLAGS = $(ROOT_CFLAGS)
LIBDEST = ../../

$(TARGET).so: $(SOURCE:.cc=.o) $(DICT:.cc=.o)
	$(CXX) --shared -fPIC -o $@ $(ROOT_LIBS) $^ 
	@mkdir -p $(LIBDEST)
	@ln -sf $(shell pwd)/$(TARGET).so $(LIBDEST)

$(DICT): $(HEADER)
	rootcint -f $@ -c $^ $(LINKDEF)
	$(CXX) -c -fPIC $@ $(FLAGS)

clean:
	@rm -rf $(DICT:.cc=.*)
	@rm -rf *.o
	@rm -rf $(TARGET).so $(LIBDEST)/$(TARGET).so

%.o: %.cc
	$(CXX) -o $@ -c -fPIC $(FLAGS) $<
