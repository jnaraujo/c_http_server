#include "Server.h"

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib,"Ws2_32.lib")

Server Server_start(Router * router, int port) {
  SOCKET sock, msgsock;
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

  while(1) {
    msgsock = accept(sock, NULL, NULL);

    if(msgsock == INVALID_SOCKET) {
      printf("accept() failed: %d\n", WSAGetLastError());
      WSACleanup();
      exit(1);
    }

    char buffer[1024];
    int bytes;

    bytes = recv(msgsock, buffer, sizeof(buffer), 0);

    if(bytes == SOCKET_ERROR) {
      printf("recv() failed: %d\n", WSAGetLastError());
      WSACleanup();
      exit(1);
    }

    buffer[bytes] = '\0';

    // get route
    char* path = strtok(buffer, " ");
    path = strtok(NULL, " ");

    Route* route = Router_getRoute(router, path);

    printf("Path: %s\n", path);

    if (route == NULL) {
      printf("Route not found\n");
      closesocket(msgsock);
      continue;
    }

    char http_header[4096] = "HTTP/1.1 200 OK\r\n\r\n";

    char* html = read_html_file(route->html);

    strcat(http_header, html);
    // strcat(http_header, "\r\n\r\n");


    // strcat(response, "\r\n\r\n");

    printf("Response: %s \n", http_header);
    

    send(msgsock, http_header, strlen(http_header), 0);
    closesocket(msgsock);

    free(html);
  }

  closesocket(sock);
  WSACleanup();


  return (Server) { .socket = sock };

}