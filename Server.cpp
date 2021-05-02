/*
 * Server.cpp
 *
 * Author: 318948809 Liad Malihi
 */

#include "Server.h"
//constructor- creat a socket, bind and listen t0 this socket.
Server::Server(int port)throw (const char*) {
	fd=socket(AF_INET,SOCK_STREAM,0);
	if(fd<0){
		throw "socket failed";
	}
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(port);
	
	if(bind(fd,(struct sockaddr*) &server, sizeof(server))<0){
  		throw "bind failed";
 	}
 	if(listen(fd,5)<0){
  		throw "listen failed";
	}
	keep=true;
}
//this function get a id client, and sent the id to the function handle.
void Server::start(ClientHandler& ch)throw(const char*){
	// open thread
	t=new thread([&ch,this](){
		while(keep){
			socklen_t clientSize = sizeof(client);
			alarm(7);
			int a_client = accept(fd,(struct sockaddr*)&client,&clientSize); //  id of client
			if(a_client <0){
				throw "accept failed"; // the accept failed
			}
			ch.handle(a_client);
			close(a_client); // close the connection with this client
		}
		close(fd);
	});	
}
//this function stops the listen to clients.
void Server::stop(){
	keep=false; 
	t->join(); // do not delete this!
}

Server::~Server() {
}

