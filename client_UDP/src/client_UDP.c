/*
 ============================================================================
 Name        : UDP_exercise.c
 Author      : Serio Luca
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "client.h"

int main(int argc, char *argv[]) {
	setbuf(stdout, NULL);

	int sock = 0;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	calculator cal;
	unsigned int fromSize;
	int echoStringLen;
	//necessary to validate port and IP
	conn info;
	struct hostent *host;
	char string1[30], string2[30], string3[30];

	//parameters check
	if (argc == 2) {
		char *p = strtok(argv[1], (char*) ":");
		strcpy(info.name, p);
		if (p == NULL) {
			error_handler("Format incorrect");
			get_name(info.name);
			get_port(&info.port);
		} else {
			p = strtok(NULL, (char*) ":");
			info.port = atoi(p);
		}

	} else if (argc == 1) { //default case
		strcpy(info.name, NAME);
		info.port = PORT;
	} else {
		get_name(info.name);
		get_port(&info.port);
	}
	validation(&info);
//C:\Users\serio\eclipse-workspace\client_UDP\Debug
#if defined WIN32
	if (!creation_wsa()) {
		error_handler("error at WSASturtup\n");
		return 0;
	}
#endif
	//socket creation
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		error_handler("socket() failed");
		return 0;
	}
	//server creation
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET;
	echoServAddr.sin_port = htons(info.port);
	getip(host, &info);
	echoServAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*info.ip));

	while (1) {
		do {
			printf("Insert a valid operation ex.+ 1 2\n");
			fflush(stdin);
			scanf("%s %s %s", &string1[0], &string2[0], &string3[0]);

			if (!ischar(string1))
				puts("\nerrore inserimento carattere");
			if (!is_number(string2))
				puts("\nerrore inserimento primo numero");
			if (!is_number(string3))
				puts("\nerrore inserimento secondo numero\n");

		} while (!ischar(string1) || !is_number(string2) || !is_number(string3));
		cal.op = string1[0];
		if (cal.op != '=') {
			cal.num1 = atoi((char*) string2);
			cal.num2 = atoi((char*) string3);

			echoStringLen = sizeof(cal);
			//send of request
			cal.num1 = htonl(cal.num1);
			cal.num2 = htonl(cal.num2);
			if (sendto(sock, (void*) &cal, echoStringLen, 0,
					(struct sockaddr*) &echoServAddr, sizeof(echoServAddr))
					!= echoStringLen) {
				error_handler(
						"sendto() sent different number of bytes than expected.\n");
				break;
			}

			//recdive of result
			fromSize = sizeof(fromAddr);
			recvfrom(sock, (void*) &cal, sizeof(cal), 0,
					(struct sockaddr*) &fromAddr, (int*) &fromSize);
			if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
				error_handler(
						"Error: received a packet from unknown source.\n");
				break;
			} else {
				getip(host, &info);
				cal.num1 = ntohl(cal.num1);
				cal.num2 = ntohl(cal.num2);
				printf("Result received from the server  ");
				getname(host, echoServAddr.sin_addr);
				printf(" with IP %s : %d %c %d = %s \n", inet_ntoa(*info.ip),
						cal.num1, cal.op, cal.num2, cal.res);
			}
		} else {
			break;
		}

	}
	system("pause");
	closesocket(sock);
	return 0;
}
