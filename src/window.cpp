#include "window.h"

int XWindow::create(int width, int height, const char *title)
{
    int err = EXIT_SUCCESS;

    if (display || screen || root || window)
        err = ERR_WND_CPY;

    if (!err)
    {
        display = XOpenDisplay(nullptr);
        if (!display) err = ERR_WND_DSPL;
    }
    
    if (!err)
    {
        screen = DefaultScreen(display);
        if (screen < 0) err = ERR_WND_SCRN;
    }
    
    if (!err)
    {
        root = RootWindow(display, screen);
        if (!root) err = ERR_WND_ROOT;
    }
    
    if (!err)
    {
        window = XCreateSimpleWindow(display, root, 0, 0, width, height, 1,
                                    BlackPixel(display, screen),
                                    WhitePixel(display, screen));

        if (!window) err = ERR_WND_WND;
    }
    
    if (!err && !XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask))
    {
        err = ERR_WND_SLCT_INPT;
    }
    
    if (!err && !XStoreName(display, window, title))
    {
        err = ERR_WND_STORE_NAME;
    }
    
    if (!err && !XMapWindow(display, window))
    {
        err = ERR_WND_MAP_WND;
    }

    return err;
}

void XWindow::event_loop(void)
{
    XEvent event;
    Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);
    KeySym key_sym;
    bool loop_running = true;

    while (loop_running)
    {
        // Проверяем, нужно ли закрыть окно перед получением следующего события
        if (XPending(display) > 0) 
        {
            XNextEvent(display, &event);

            switch (event.type)
            {
            case Expose:
                // Обработка события Expose (перерисовка окна)
                break;

            case KeyPress:
            {
                // Обработка события KeyPress (нажатие клавиши)
                key_sym = XLookupKeysym(&event.xkey, 0);

                switch (key_sym)
                {
                case XK_Escape:
                    loop_running = false;
                    break;
                
                default:
                    break;
                }

                break;
            }
            case DestroyNotify:
                loop_running = false;
                break;

            default:
                break;
            }
        }
    }
}

void XWindow::destroy(void)
{
    if (display)
    {
        Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", False);

        XEvent ev;
        ev.type = ClientMessage;
        ev.xclient.window = window;
        ev.xclient.message_type = XInternAtom(display, "WM_PROTOCOLS", False);
        ev.xclient.format = 32;
        ev.xclient.data.l[0] = WM_DELETE_WINDOW;

        XSendEvent(display, window, False, NoEventMask, &ev);
        XFlush(display);

        // Ожидание завершения обработки сообщения о закрытии окна
        XEvent event;
        while (true)
        {
            if (XCheckTypedEvent(display, ClientMessage, &event))
            {
                if (event.xclient.data.l[0] == WM_DELETE_WINDOW)
                    break;
            }
            // Можно добавить другие проверки или условия выхода из цикла ожидания
        }

        XDestroyWindow(display, window);
        XCloseDisplay(display);
    }

    display = nullptr;
    screen = 0;
    root = 0;
    window = 0;
}
