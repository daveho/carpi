CXX = g++

SRCS = main.cpp mainwin.cpp
OBJS = $(SRCS:%.cpp=%.o)

GTK3_FLAGS = `pkg-config --cflags gtk+-3.0`
GTK3_LIBS = `pkg-config --libs gtk+-3.0`
CXXFLAGS = -g -Wall $(GTK3_FLAGS)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $*.o

carpi_ui : $(OBJS)
	$(CXX) -o $@ $(OBJS) $(GTK3_LIBS)

depend :
	$(CXX) $(CXXFLAGS) -M $(SRCS) > depend.mak

depend.mak :
	touch $@

include depend.mak
