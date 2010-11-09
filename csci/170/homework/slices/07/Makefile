CPP=g++
SLICE=S7
all: 1 2
1: $(SLICE).E1.1.cpp
	$(CPP) $(SLICE).E1.1.cpp -o ~/public_html/cgi-bin/$(SLICE).E1.1.cgi
2: $(SLICE).E1.21.cpp $(SLICE).E1.22.cpp
	$(CPP) $(SLICE).E1.21.cpp -o ~/public_html/cgi-bin/$(SLICE).E1.2-read.cgi
	$(CPP) $(SLICE).E1.22.cpp -o ~/public_html/cgi-bin/$(SLICE).E1.2-write.cgi
clean:
	rm ~/public_html/cgi-bin/$(SLICE).E1.1.cgi
	rm ~/public_html/cgi-bin/$(SLICE).E1.2-*.cgi
