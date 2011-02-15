###############################################################################
# Makefile
# Shaun Meyer, Feb 2011
###############################################################################

CPPFLAGS += -I/usr/local/include
CXXFLAGS += -g 
CXXFLAGS += $(CPPFLAGS)

#.SUFFIXES: .o .cpp
#.cpp.o:
#	$(CXX) $(CXXFLAGS) -c $> -o $@

#adjmatrix: adjMatrix.cpp adjMatrix.hpp
#	g++ -g -c $(INC) adjMatrix.cpp -o adjMatrix

bitTest: adjMatrix.o bitarray.o bitTest.o 
	$(CXX)	bitarray.o adjMatrix.o bitTest.o -o bitTest

btest: adjMatrix.o bitarray.o btest.cpp
	$(CXX) $(CXXFLAGS) bitarray.o adjMatrix.o btest.cpp -o btest

install:
	cp sqtest ~/exec/

clean:
	rm *.o btest bitTest
