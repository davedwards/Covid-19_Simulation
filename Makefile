CXX ?= gcc
PKGCONFIG = $(shell which pkg-config)
CXXFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0) -Wall -g 
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0) -lbsd

SRC = simulator.cpp location.cpp  mobility_model.cpp  person.cpp  ppmodel.cpp  ui.cpp

OBJS = $(SRC:.cpp=.o)

all: simulator

%.o: %.cpp
	$(CXX) -c -o $(@F) $(CXXFLAGS) $<

simulator: $(OBJS)
	$(CXX) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f simulator