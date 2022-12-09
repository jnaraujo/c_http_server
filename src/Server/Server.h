#include "../Router/Router.h"
#include <winsock2.h>

#ifndef SERVER_H
#define SERVER_H

SOCKET Server_start(Router * router, int port);

#endif