/*
 ============================================================================
 Name        : UDP_server.c
 Author      : Serio Luca
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "server.h"

int main(int argc, char **argv) {
#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("error at WSASturtup\n");
		return EXIT_FAILURE;
	}
#endif
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	struct hostent *host;
	struct in_addr address;
	calculator cal;
	unsigned int cliAddrLen;
	unsigned int calcSize;

	//socket creation
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		puts("socket() failed");
	// definition of server
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_port = htons(PORT);
	echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//socket bind
	if ((bind(sock, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr)))
			< 0)
		puts("bind() failed");

	while (1) {
		//receive of request
		cliAddrLen = sizeof(echoClntAddr);
		recvfrom(sock, (void*) &cal, sizeof(cal), 0,
				(struct sockaddr*) &echoClntAddr, (int*) &cliAddrLen);
		cal.num1 = ntohl(cal.num1);
		cal.num2 = ntohl(cal.num2);
		//resolution name host by IP
		address.s_addr = inet_addr((char*) inet_ntoa(echoClntAddr.sin_addr));
		host = gethostbyaddr((char*) &address, 4, AF_INET);
		char *name = host->h_name;
		printf("Richiesta operazione: '%c %d %d' dal client: "
				"%s, con IP: %s\n", cal.op, cal.num1, cal.num2, name,
				inet_ntoa(echoClntAddr.sin_addr));

		//operation switch
		switch (cal.op) {
		case '+':
			snprintf(cal.res, 10, "%d", add(cal.num1, cal.num2));
			break;
		case '-':
			snprintf(cal.res, 10, "%d", sub(cal.num1, cal.num2));
			break;
		case '*':
			snprintf(cal.res, 10, "%d", mult(cal.num1, cal.num2));
			break;
		case '/':
			division(&cal);
			break;
		}

		calcSize = sizeof(cal);
		cal.num1 = htonl(cal.num1);
		cal.num2 = htonl(cal.num2);
		if (sendto(sock, (void*) &cal, calcSize, 0,
				(struct sockaddr*) &echoClntAddr, sizeof(echoClntAddr))
				!= calcSize)
			errorhandler("SendTo error!");
	}
}
