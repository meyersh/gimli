############################
## Project 2 - CSCI 270
## SHAUN MEYER
################## APR, 2010
############## REV OCT, 2010
#

#CPP=g++ -g
SLICE=S13
OBJECTS=database.o cookies.o session.o randomStr.o messaging.o
INSTALL_PATH=~/public_html/270/social

CXXFLAGS += -Wall -g -L/usr/local/lib/mysql -I/usr/local/include/mysql
LDFLAGS += -lmysqlclient
# This is a cool inference, compile all *.hpp & *.cpp into *.o files:
#%.o: %.cpp %.hpp
#$(CPP) $(CPPFLAGS) -O2 -c $<

all: logout.cgi checksession.cgi login.cgi home.cgi db-drv

logout.cgi: $(OBJECTS) logout.o

checksession.cgi: $(OBJECTS) checksession.o 

login.cgi: login.o cookies.o session.o database.o

home.cgi: $(OBJECTS) home.o

db-drv: db.cpp db.hpp database.hpp database.o

# remove all *.o files - need to sometimes.
clean:
	-rm *.o

# install all cgi's. This will also build dependencies
install: all
	cp login.cgi $(INSTALL_PATH)
	cp checksession.cgi $(INSTALL_PATH)
	cp logout.cgi $(INSTALL_PATH)
	cp home.cgi $(INSTALL_PATH)

# show a count of the lines of source code (scary!)
count:
	wc -l *.cpp *.hpp  html/js/ajax.js html/*.html 

# remove OUR cgi's
remove:
	rm $(INSTALL_PATH)/login.cgi
	rm $(INSTALL_PATH)/checksession.cgi
	rm $(INSTALL_PATH)/logout.cgi
	rm $(INSTALL_PATH)/home.cgi
