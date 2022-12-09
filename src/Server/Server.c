#include "Server.h"

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib,"Ws2_32.lib")

SOCKET Server_start(Router * router, int port) {
  SOCKET sock;
  WSADATA wsadata;

  SOCKADDR_IN serverAddr;

  if(WSAStartup(MAKEWORD(1, 1), &wsadata) != 0) {
    printf("WSAStartup failed.\n");
    exit(1);
  }

  sock = socket(AF_INET, SOCK_STREAM, 0);

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(port);

  if(sock == INVALID_SOCKET) {
    printf("socket() failed: %d\n", WSAGetLastError());
    WSACleanup();
    exit(1);
  }

  if(bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) != 0) {
    printf("bind() failed: %d\n", WSAGetLastError());
    WSACleanup();
    exit(1);
  }

  if(listen(sock, 5) == SOCKET_ERROR) {
    printf("listen() failed: %d\n", WSAGetLastError());
    WSACleanup();
    exit(1);
  }

  printf("Listening on port %d\n", port);


  return sock;
}