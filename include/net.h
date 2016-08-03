#include "include.h"

#ifndef NET_H
#define NET_H

#ifdef __linux__
#define SOCKET unsigned int
#endif

int inet_pton(int af, const char *server, struct in_addr *addr);

/*
	UDP non blocking
*/
class Net
{
public:
	void connect(char *server, int port);
	void bind(char *address, int port);
	void accept();
	int send(char *buffer, int size);
	int sendto(char *buff, int size, char *to);
	int recv(char *buff, int size);
	int recv(char *buffer, int size, int delay);
	int recvfrom(char *buff, int size, char *from, int length);
	void strtoaddr(char *str, struct sockaddr_in &addr);

	struct sockaddr_in	servaddr;
	SOCKET			sockfd;
private:


};

#endif
