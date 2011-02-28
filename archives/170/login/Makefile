CPP=g++
SLICE=S13
OBJECTS=cookies.o session.o randomStr.o
all: logout.cgi checksession.cgi login.cgi home.cgi

logout.cgi: $(OBJECTS) logout.o
	$(CPP) logout.o $(OBJECTS) -o logout.cgi

checksession.cgi: $(OBJECTS) checksession.o 
	$(CPP) checksession.o $(OBJECTS) -o checksession.cgi

login.cgi: $(OBJECTS) login.o
	$(CPP) $(OBJECTS) login.o -o login.cgi

home.cgi: $(OBJECTS) home.o
	$(CPP) $(OBJECTS) home.o -o home.cgi
# This is a cool inference, compile all *.hpp & *.cpp into *.o files:
%.o: %.cpp %.hpp
	$(CPP) -O2 -c $<

clean:
	rm *.o
install: login.cgi checksession.cgi logout.cgi
	cp login.cgi ~/public_html/
	cp checksession.cgi ~/public_html/
	cp logout.cgi ~/public_html/
	cp home.cgi ~/public_html/
count:
	wc checksession.cpp cookies.cpp cookies.hpp login.cpp Makefile session.cpp session.hpp ~/public_html/ajax.js ~/public_html/profile.html ~/public_html/login.html
