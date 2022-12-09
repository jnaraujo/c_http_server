#include "../Router/Router.h"

#ifndef SERVER_H
#define SERVER_H

typedef struct {
  SOCKET socket;
} Server;

Server Server_start(Router * router, int port);

#endif