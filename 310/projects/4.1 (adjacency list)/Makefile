BOOST=-I/usr/local/include
HELPERS=shaun.o bitarray.o adjmatrix.o matrix_utils.o sysutils.o

all: bfs-driver sysutils-driver dot.cgi load_graph.cgi save_graph.cgi graph.cgi list_graphs.cgi dot_cli

bitarray.o: adjmatrixABC/bitarray.cpp
	g++ $(BOOST) -c adjmatrixABC/bitarray.cpp -o bitarray.o

adjmatrix.o: adjmatrixABC/adjMatrix.cpp bitarray.o
	g++ $(BOOST) -c adjmatrixABC/adjMatrix.cpp -o adjmatrix.o

matrix_utils.o: shaun.o
	g++ $(BOOST) -c matrix_utils.cpp

bfs-driver:  adjmatrix.o matrix_utils.o bfs-driver.cpp shaun.o
	g++ $(BOOST) $(HELPERS)  bfs-driver.cpp -o bfs-driver

sysutils-driver: shaun.o sysutils.o sysutils-driver.cpp
	g++ $(BOOST) $(HELPERS) sysutils-driver.cpp -o sysutils-driver

dot.cgi: $(HELPERS) dot_cgi.cpp
	g++ $(BOOST) $(HELPERS) dot_cgi.cpp -o dot.cgi

dot_cli: $(HELPERS) dot_cli.cpp 
	g++ $(BOOST) $(HELPERS) dot_cli.cpp -o dot_cli

load_graph.cgi: $(HELPERS) load_graph_cgi.cpp
	g++ $(BOOST) $(HELPERS) load_graph_cgi.cpp -o load_graph.cgi

save_graph.cgi: $(HELPERS) save_graph_cgi.cpp
	g++ $(BOOST) $(HELPERS) save_graph_cgi.cpp -o save_graph.cgi

graph.cgi: $(HELPERS) graph_cgi.cpp
	g++ $(BOOST) $(HELPERS) graph_cgi.cpp -o graph.cgi

list_graphs.cgi: $(HELPERS) list_graphs_cgi.cpp
	g++ $(BOOST) $(HELPERS) list_graphs_cgi.cpp -o list_graphs.cgi

clean:
	rm *.o *.cgi sysutils-driver bfs-driver dot_cli

install: all
	cp *.cgi ~/public_html/310/bfs/
	cp webdot_cgi.sh ~/public_html/310/bfs/webdot.cgi
	cp dot_cli ~/bin/
