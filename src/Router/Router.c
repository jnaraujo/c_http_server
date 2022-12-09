#include "Router.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Router* Router_new() {
  Router* router = malloc(sizeof(Router));

  router->routes = NULL;
  router->routesCount = 0;

  return router;
}

void Router_addRoute(Router* router, char* path, char* html) {
  Route* route = malloc(sizeof(Route));
  route->path = path;
  route->html = html;

  router->routes = realloc(router->routes, sizeof(Route*) * (router->routesCount + 1));

  router->routes[router->routesCount] = route;
  router->routesCount++;
}

Route* Router_getRoute(Router* router, char* path) {
  for (int i = 0; i < router->routesCount; i++) {
    char* routePath = router->routes[i]->path;

    if (strcmp(router->routes[i]->path, path) == 0) {
        return router->routes[i];
    }
  }

  return NULL;
}