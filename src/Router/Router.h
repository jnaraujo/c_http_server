#ifndef ROUTER_H
#define ROUTER_H

typedef struct {
    char *path;
    char* html;
} Route;

typedef struct {
    Route **routes;
    int routesCount;
} Router;

Router *Router_new();

void Router_addRoute(Router* router, char* path, char* html);

Route* Router_getRoute(Router *, char *);

#endif