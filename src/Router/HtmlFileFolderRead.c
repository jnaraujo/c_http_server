#include "HtmlFileFolderRead.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


HtmlFiles ReadFilesOnPath(const char* path){
  WIN32_FIND_DATA fdFile;
  HANDLE hFind = NULL;

  char sPath[2048];

  HtmlFiles htmlFiles;
  htmlFiles.count = 0;
  htmlFiles.files = NULL;

  sprintf(sPath, "%s\\*.*", path);

  if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE){
    printf("Path not found: [%s]\n", sPath);
    return htmlFiles;
  }

  

  do{
    if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0){
      char* filePath = malloc(strlen(path) + strlen(fdFile.cFileName) + 2);
      strcpy(filePath, path);
      strcat(filePath, "\\");
      strcat(filePath, fdFile.cFileName);

      htmlFiles.files = realloc(htmlFiles.files, sizeof(char*) * (htmlFiles.count + 1));
      htmlFiles.files[htmlFiles.count] = filePath;
      htmlFiles.count++;
    }
  }while(FindNextFile(hFind, &fdFile));

  FindClose(hFind);

  return htmlFiles;
}