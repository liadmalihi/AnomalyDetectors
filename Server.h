
/*
 * Server.h
 *
 * Author: 318948809 Liad Malihi
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "commands.h"
#include "CLI.h"

using namespace std;

//  interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class SocketIO:public DefaultIO{
	int client_id;
public:
	SocketIO(int id_client){
		client_id=id_client;
	}

	virtual string read(){
		string serverInput="";
		char c=0;
		recv(client_id,&c,sizeof(char),0); // read from client
		while(c!='\n'){				
			serverInput+=c;
			recv(client_id,&c,sizeof(char),0);
		}
		return serverInput;
	}
	virtual void write(string text){
		send(client_id,text.c_str(),text.length(),0);
	}

	virtual void write(float f){
		string f_string=to_string(f);
		f_string.erase(f_string.find_last_not_of('0') + 1, std::string::npos); // string without 0
		send(client_id,f_string.c_str(),f_string.length(),0);
	}

	virtual void read(float* f){
		string serverInput="";
		char c=0;
		recv(client_id,&c,sizeof(char),0);
		while(c!='\n'){				
			serverInput+=c;
			recv(client_id,&c,sizeof(char),0);
		}
		*f= stof(serverInput); // from string to float*
	}
};
// you can add helper classes here and implement on the cpp file


//  AnomalyDetectionHandler class
//// this class sends the client to the handle in a socket.
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
		SocketIO client(clientID);
		CLI cli(&client);
		cli.start();
    }
};


// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
	int fd;
	sockaddr_in server;
	sockaddr_in client;
	bool keep;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
