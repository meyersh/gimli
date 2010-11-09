CPP=g++
SLICE=S13
all: tstamp.o randomStr.o combo.o
	$(CPP) tstamp.o randomStr.o combo.o -o combo
randomStr.o: randomStr.cpp
	$(CPP) -O2 -c randomStr.cpp
tstamp.o: tstamp.cpp
	$(CPP) -O2 -c tstamp.cpp
combo.o: combo.cpp
	$(CPP) -O2 -c combo.cpp
clean:
	rm *.o