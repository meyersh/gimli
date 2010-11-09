# ###########################################
# mysql stuff
# Shaun Meyer, 2010
# ###########################################

mysqlib = -L/usr/lib/mysql
mysqlinc = -I/usr/include/mysql
mysqliblk = -lmysqlclient


mysql-exercise.x: mysql-exercise.cpp
	g++ $(mysqlib) $(mysqlinc) -o mysql-exercise.x mysql-exercise.cpp $(mysqliblk)

