#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include "definers.h"

class XWindow
{
public:
    Display *display = nullptr;
    int screen = 0;
    XID root = 0;
    XID window = 0;
public:
    int create(int width, int height, const char *title);

    void event_loop(void);

    void destroy(void);
};

#endif // WINDOW_H