CPP=g++
SLICE=S13
all: cookies.o checksession.o randomStr.o session.o
	$(CPP) -O2 session.o cookies.o checksession.o randomStr.o -o checksession.cgi

randomStr.o: randomStr.cpp randomStr.hpp
	$(CPP) -O2 -c  randomStr.cpp
cookies.o: cookies.cpp cookies.hpp
	$(CPP) -O2 -c  cookies.cpp
checksession.o: checksession.cpp 
	$(CPP) -O2 -c  checksession.cpp
sesson.o: session.cpp session.hpp
	$(CPP) -O2 -c session.cpp 
clean:
	rm *.o
install:
	cp checksession.cgi ~/public_html/
count:
	wc checksession.cpp cookies.cpp cookies.hpp login.cpp Makefile session.cpp session.hpp
