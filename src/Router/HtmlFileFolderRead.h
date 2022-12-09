#ifndef HTMLFILEFOLDERREAD_H
#define HTMLFILEFOLDERREAD_H

typedef struct {
  char** files;
  int count;
} HtmlFiles;

HtmlFiles ReadFilesOnPath(char*);

#endif

