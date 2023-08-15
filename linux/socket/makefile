all:server client multiprocess multithread

server:server.cc 
	g++ -g -o server.exe server.cc myTcp.h myTcp.cpp

client:client.cc
	g++ -g -o client.exe client.cc myTcp.h myTcp.cpp

multiprocess:multiprocess.cc
	g++ -g -o mp.exe multiprocess.cc myTcp.h myTcp.cpp

multithread:multithread.cc
	g++ -g -o mt.exe multithread.cc myTcp.h myTcp.cpp -lpthread
	