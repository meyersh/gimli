adjmatrix.o:
	g++ -c adjmatrix/adjmatrix.cpp

matrix_utils.o: shaun.o
	g++ -I/usr/local/include -c matrix_utils.cpp

bfs-driver: adjmatrix.o matrix_utils.o bfs-driver.cpp shaun.o
	g++ -I/usr/local/include adjmatrix.o matrix_utils.o shaun.o bfs-driver.cpp -o bfs-driver

sysutils-driver: sysutils.o sysutils-driver.cpp
	g++ -I/usr/local/include sysutils.o sysutils-driver.cpp -o sysutils-driver

dot.cgi: dot_cgi.cpp adjmatrix.o shaun.o
	g++ -I/usr/local/include shaun.o adjmatrix.o matrix_utils.o dot_cgi.cpp -o dot.cgi