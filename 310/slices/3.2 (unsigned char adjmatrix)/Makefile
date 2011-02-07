INC="/usr/local/include"
adjmatrix: adjMatrix.cpp adjMatrix.hpp
	g++ -g -c -I$(INC) adjMatrix.cpp -o adjMatrix

btest: adjMatrix.o btest.cpp
	g++ -g -I$(INC) adjMatrix.o btest.cpp -o btest
install:
	cp sqtest ~/exec/
