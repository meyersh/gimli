LIBS=adjmatrix/adjmatrix.o

dist.x: $(LIBS) dist.o
	g++ $(LIBS) dist.o -o dist.x

clean:
	find . -name "*.o" -delete

install:
	cp dist.x ~/exec/dist.x