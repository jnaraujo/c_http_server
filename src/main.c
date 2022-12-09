#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <winsock2.h>

#include "Router/Router.h"
#include "Server/Server.h"
#include "Router/HtmlFileFolderRead.h"

char* read_html_file(char* path) {
  FILE* file = fopen(path, "r");

  if (file == NULL) {
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fsize = ftell(file);
  rewind(file);


  char* temp = malloc(sizeof(char) * (fsize+1));
  char c;

  int i = 0;
  while((c = fgetc(file)) != EOF) {
    temp[i] = c;
    i++;
  }

  temp[i] = '\0';

  fclose(file);

  return temp;
}

char* getHtmlFilePath(){
    char* currentFile = __FILE__;
    char* currentDir = malloc(strlen(currentFile) + 1);
    strcpy(currentDir, currentFile);

    char* htmlDirPath = malloc(strlen(currentDir) + 1);
    strcpy(htmlDirPath, currentDir);

    char* lastSlash = strrchr(htmlDirPath, '\\');
    *lastSlash = '\0';

    lastSlash = strrchr(htmlDirPath, '\\');
    *lastSlash = '\0';

    strcat(htmlDirPath, "\\html");

    return htmlDirPath;
}

int main() {
    Router* router = Router_new();

    char* htmlDirPath = getHtmlFilePath();

    printf("Html dir path: %s\n", htmlDirPath);

    HtmlFiles htmlFiles = ReadFilesOnPath(htmlDirPath);

    printf("Count of files: %d\n", htmlFiles.count);

    for (int i = 0; i < htmlFiles.count; i++) {
        char* filePath = htmlFiles.files[i];
        char* routerName = strrchr(filePath, '\\');

        routerName[0] = '/';

        char* routeNameWithoutHtml = malloc(strlen(routerName) + 1);
        strcpy(routeNameWithoutHtml, routerName);

        char* lastDot = strrchr(routeNameWithoutHtml, '.');
        *lastDot = '\0';

        printf("Route name: %s\n", routeNameWithoutHtml);

        Router_addRoute(router, routeNameWithoutHtml, filePath);
    }

    SOCKET msgsock;
    SOCKET socket =  Server_start(router, 3000); 

    while(1) {
        msgsock = accept(socket, NULL, NULL);

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

        send(msgsock, http_header, strlen(http_header), 0);
        closesocket(msgsock);
        free(html);
    }

    closesocket(socket);
    WSACleanup();
}