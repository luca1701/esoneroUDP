/*
 * server.h
 *
 *  Created on: 9 dic 2021
 *      Author: serio
 */

#ifndef SERVER_H_
#define SERVER_H_

#ifdef WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#endif
#define PORT 50000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int num1;
	int num2;
	char res[10];
	char op;
} calculator;

int add(int a, int b) {
	return a + b;
}
int mult(int a, int b) {
	return a * b;
}
int sub(int a, int b) {
	return a - b;
}

void division(calculator *c) {
	if (c->num2 != 0) {
		snprintf(c->res, 10, "%f", (float) c->num1 / (float) c->num2);
	} else
		strcpy(c->res, "Error!");
}

void errorhandler(char *errormessage) {
	printf(errormessage);
}

void clearwinsock() {
#ifdef WIN32
	WSACleanup();
#endif
}

#endif /* SERVER_H_ */
