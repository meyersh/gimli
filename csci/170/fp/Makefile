############################
##### FINAL PROJECT MAKEFILE
## SHAUN MEYER, CSCI-170-001
################## APR, 2010
#

CPP=g++
SLICE=S13
OBJECTS=cookies.o session.o randomStr.o messaging.o 

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

# remove all *.o files - need to sometimes.
clean:
	rm *.o

# install all cgi's. This will also build dependencies
install: login.cgi checksession.cgi logout.cgi
	cp login.cgi ~/public_html/fp/
	cp checksession.cgi ~/public_html/fp/
	cp logout.cgi ~/public_html/fp/
	cp home.cgi ~/public_html/fp/

# show a count of the lines of source code (scary!)
count:
	wc *.cpp *.hpp  ~/public_html/fp/js/ajax.js ~/public_html/fp/*.html 

# remove OUR cgi's
remove:
	rm ~/public_html/fp/login.cgi
	rm ~/public_html/fp/checksession.cgi
	rm ~/public_html/fp/logout.cgi
	rm ~/public_html/fp/home.cgi