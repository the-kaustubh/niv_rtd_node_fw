#ifndef _ROUTES_H
#define _ROUTES_H

#include "global.h"

void handleRoot() {
  server.send(200, "text/html", "<b>Hello </b>");

}

void handleNotFound() {
  server.send(404, "text/html", "<b>Not Found</b>");
}

#endif
