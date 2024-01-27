#ifndef CONSOLE_H
#define CONSOLE_H

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN 1 // Урезанный windows.h
#include <Windows.h>
#include <io.h>
#include <iostream>
#include <stdio.h>

#define MIN_BUFFER_SIZE 128

class Console
{
private:
    bool redirect_io() const;

    void adjust_buffer() const;

    static BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType);

public:
    Console(){};
    ~Console(){};

    bool create() const;

    bool destroy() const;

    void set_size(INT width, INT height) const;

    void set_pos(INT x, INT y) const;

    void set_curet_pos(SHORT x, SHORT y) const;

    int get_curet_row(void) const;

    int get_curet_col(void) const;

    void get_curet_pos(int *row, int *col) const;
};

#endif // CONSOLE_H