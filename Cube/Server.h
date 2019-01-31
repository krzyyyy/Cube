#pragma once
#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512	//Max length of buffer

#define PORT 5000	//The port on which to listen for incoming data

class Server {
private:
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;
public:
	Server();
	void reciveData(std::string &data);
	~Server();

};