CPP=g++
SLICE=S6
all: 1 2 3
1: $(SLICE).E1.1.cpp
	$(CPP) $(SLICE).E1.1.cpp -o $(SLICE).E1.1
2: $(SLICE).E1.2.cpp
	$(CPP) $(SLICE).E1.2.cpp -o $(SLICE).E1.2
3: $(SLICE).E1.31.cpp $(SLICE).E1.32.cpp
	$(CPP) $(SLICE).E1.31.cpp -o $(SLICE).E1.31
	$(CPP) $(SLICE).E1.32.cpp -o $(SLICE).E1.32
clean:
	rm $(SLICE).E1.[1-3]
