############################
## Project 2 - CSCI 270
## SHAUN MEYER
################## APR, 2010
############## REV OCT, 2010
#

CPP=g++ -g
SLICE=S13
OBJECTS=database.o cookies.o session.o randomStr.o messaging.o
INSTALL_PATH=~/public_html/270/social

# MYSQL STUFF:
CPPFLAGS=-g -L/usr/lib/mysql -I/usr/include/mysql -lmysqlclient
mysql=$(CPPFLAGS)

# This is a cool inference, compile all *.hpp & *.cpp into *.o files:
%.o: %.cpp %.hpp
	$(CPP) $(CPPFLAGS) -O2 -c $<

all: logout.cgi checksession.cgi login.cgi home.cgi db-drv

logout.cgi: $(OBJECTS) logout.o
	$(CPP) $(mysql) logout.o $(OBJECTS) -o logout.cgi

checksession.cgi: $(OBJECTS) checksession.o 
	$(CPP) $(mysql) checksession.o $(OBJECTS) -o checksession.cgi

login.cgi: $(OBJECTS)
	$(CPP) $(mysql) $(OBJECTS) login.cpp -o login.cgi

home.cgi: $(OBJECTS) home.o
	$(CPP) $(mysql) $(OBJECTS) home.o -o home.cgi

db-drv: db.cpp db.hpp database.hpp database.o
	$(CPP)  $(mysql) db.cpp database.o -o db-drv

# remove all *.o files - need to sometimes.
clean:
	rm *.o

# install all cgi's. This will also build dependencies
install: all
	cp login.cgi $(INSTALL_PATH)
	cp checksession.cgi $(INSTALL_PATH)
	cp logout.cgi $(INSTALL_PATH)
	cp home.cgi $(INSTALL_PATH)

# show a count of the lines of source code (scary!)
count:
	wc *.cpp *.hpp  ~/public_html/fp/js/ajax.js ~/public_html/fp/*.html 

# remove OUR cgi's
remove:
	rm $(INSTALL_PATH)/login.cgi
	rm $(INSTALL_PATH)/checksession.cgi
	rm $(INSTALL_PATH)/logout.cgi
	rm $(INSTALL_PATH)/home.cgi