CPPFLAGS+=-I/usr/local/include
CXXFLAGS+=$(CPPFLAGS)
adjMatrix-driver: adjMatrix.o bitarray.o adjMatrix-driver.cpp
	g++ -I/usr/local/include bitarray.o adjMatrix.o adjMatrix-driver.cpp -o adjMatrix-driver