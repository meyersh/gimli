all: bfs-driver sysutils-driver dot.cgi

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

load_graph.cgi: load_graph_cgi.cpp adjmatrix.o matrix_utils.o shaun.o
	g++ -I/usr/local/include shaun.o adjmatrix.o matrix_utils.o load_graph_cgi.cpp -o load_graph.cgi

clean:
	rm *.o sysutils-driver dot.cgi bfs-driver