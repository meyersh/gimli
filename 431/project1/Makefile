CC=g++

include .depends 

depends: .depends

.depends: *.cpp
	g++ -MM *.cpp > .depends
