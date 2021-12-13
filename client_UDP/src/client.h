/*
 * client.h
 *
 *  Created on: 9 dic 2021
 *      Author: serio
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#ifdef WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define ECHOMAX 255
#define PORT 50000
#define NAME "localhost"

typedef struct {
	char name[30];
	int port;
	struct in_addr *ip;
	int check;
} conn;

typedef struct {
	int num1;
	int num2;
	char res[10];
	char op;
} calculator;

void get_name(char *name) {
	puts("Insert a valid nameserver Ex. uniba.it");
	scanf("%s", &*name);
}

void get_port(int *port) {
	puts("Insert a valid port number between 0 and 65536");
	scanf("%d", &*port);
}

void error_handler(char *errormessage) {
#ifdef WIN32
	WSACleanup();
#endif
	printf("%s\n", errormessage);
}

void validation(conn *info) {

	int check = 0;
	do {
		if (info->port < 0 || info->port > 65536) {
			error_handler("Port number insered doens't exists!\n");
			get_port(&info->port);
		} else if (strcmp(info->name, "localhost") == 0 && info->port == PORT) {
			check = 1;
			error_handler("\nConnection estabilished!\n\n");

		} else if (strcmp(info->name, "localhost") == 0 && info->port != PORT) {
			error_handler("\nNo server available at the port number insered\n");
			get_port(&info->port);

		} else if (strcmp(info->name, "localhost") != 0 && info->port == PORT) {
			error_handler("\nName server not solvable!\n");
			get_name(info->name);

		} else {
			error_handler("\nName server and port number unknown\n");
			get_name(info->name);
			get_port(&info->port);
		}
	} while (check == 0);
}

int creation_wsa() {
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("error at WSASturtup\n");
		return 0;
	}
	return 1;
}

void getip(struct hostent *host, conn *info) {
	host = gethostbyname((char*) info->name);
	info->ip = (struct in_addr*) host->h_addr_list[0];
}

void getname(struct hostent *host, struct in_addr address) {
	address.s_addr = inet_addr((char*) inet_ntoa(address));
	host = gethostbyaddr((char*) &address, 4, AF_INET);
	char *name = host->h_name;
	printf("%s", name);
}

int is_number(char a[30]) {
	int i = 0;
	if (a[0] == '-')
		i = 1;
	while (i < strlen(a)) {
		if (!isdigit(a[i])) {
			return 0;
		}
		i++;
	}
	return 1;
}

int ischar(char string[30]) {
	if (strlen(string) > 1) {
		return 0;
	} else {
		if (string[0] != '*') {
			if (string[0] != '/') {
				if (string[0] != '-') {
					if (string[0] != '+') {
						if (string[0] != '=') {
							return 0;
						}
					}
				}
			}
		}
		return 1;
	}
}

#endif /* CLIENT_H_ */
