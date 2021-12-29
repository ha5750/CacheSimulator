#Makefile
#Chanha Kim
#ckim135@jhu.edu

CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++11

# Source files needed only for the driver program
MAIN_SRCS = main.cpp 
MAIN_OBJS = $(MAIN_SRCS:%.cpp=build/%.o)

GAME_SRCS = block.cpp set.cpp cache.cpp
OBJS = $(GAME_SRCS:%.cpp=build/%.o)

build/%.o : %.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $*.cpp -o build/$*.o

all : csim

csim : $(MAIN_OBJS) $(OBJS)
	$(CXX) -o $@ $(MAIN_OBJS) $(OBJS)

clean :
	rm -f csim
	rm -rf build

# Running the command "make depend" will automatically generate correct
# header dependencies for all .o files
depend :
	($(CXX) $(CXXFLAGS) -MM $(MAIN_SRC) $(GAME_SRCS)  \
		| perl -ne 'if (/^[a-z]/) { print "build/"; } print') \
		> depend.mak

depend.mak :
	touch $@

include depend.mak
