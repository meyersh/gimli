all: bfs-driver sysutils-driver dot.cgi load_graph.cgi save_graph.cgi graph.cgi list_graphs.cgi dot_cli

adjmatrix.o:
	g++ -c adjmatrix/adjmatrix.cpp

matrix_utils.o: shaun.o
	g++ -I/usr/local/include -c matrix_utils.cpp

bfs-driver:  adjmatrix.o matrix_utils.o bfs-driver.cpp shaun.o
	g++ -I/usr/local/include shaun.o adjmatrix.o matrix_utils.o  bfs-driver.cpp -o bfs-driver

sysutils-driver: shaun.o sysutils.o sysutils-driver.cpp
	g++ -I/usr/local/include shaun.o sysutils.o sysutils-driver.cpp -o sysutils-driver

dot.cgi: shaun.o dot_cgi.cpp adjmatrix.o matrix_utils.o
	g++ -I/usr/local/include shaun.o adjmatrix.o matrix_utils.o dot_cgi.cpp -o dot.cgi

dot_cli: shaun.o dot_cli.cpp adjmatrix.o matrix_utils.o
	g++ -I/usr/local/include shaun.o adjmatrix.o matrix_utils.o dot_cli.cpp -o dot_cli

load_graph.cgi: load_graph_cgi.cpp adjmatrix.o matrix_utils.o shaun.o
	g++ -I/usr/local/include shaun.o adjmatrix.o matrix_utils.o load_graph_cgi.cpp -o load_graph.cgi

save_graph.cgi: save_graph_cgi.cpp adjmatrix.o matrix_utils.o shaun.o sysutils.o
	g++ -I/usr/local/include sysutils.o shaun.o adjmatrix.o matrix_utils.o save_graph_cgi.cpp -o save_graph.cgi

graph.cgi: graph_cgi.cpp adjmatrix.o matrix_utils.o shaun.o
	g++ -I/usr/local/include shaun.o adjmatrix.o matrix_utils.o graph_cgi.cpp -o graph.cgi

list_graphs.cgi: list_graphs_cgi.cpp sysutils.o shaun.o
	g++ -I/usr/local/include shaun.o sysutils.o list_graphs_cgi.cpp -o list_graphs.cgi

clean:
	rm *.o *.cgi sysutils-driver bfs-driver dot_cli

install: all
	cp *.cgi ~/public_html/310/bfs/
	cp webdot_cgi.sh ~/public_html/310/bfs/webdot.cgi
	cp dot_cli ~/bin/
