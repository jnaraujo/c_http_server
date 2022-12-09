#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>

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

    Server_start(router, 3000); 
}