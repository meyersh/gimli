CPPFLAGS += -I/usr/local/include
CXXFLAGS += -g 
CXXFLAGS += $(CPPFLAGS)

#.SUFFIXES: .o .cpp
#.cpp.o:
#	$(CXX) $(CXXFLAGS) -c $> -o $@

#adjmatrix: adjMatrix.cpp adjMatrix.hpp
#	g++ -g -c $(INC) adjMatrix.cpp -o adjMatrix

btest: adjMatrix.o btest.cpp
	$(CXX) $(CXXFLAGS) adjMatrix.o btest.cpp -o btest

bitTest: adjMatrix.o bitTest.o
	$(CXX)	adjMatrix.o bitTest.o -o bitTest

install:
	cp sqtest ~/exec/

clean:
	rm *.o btest 
