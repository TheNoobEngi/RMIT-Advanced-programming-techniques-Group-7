/*
 * platform.h - Cross-platform compatibility for keyboard input
 * Provides kbhit() and getch() functionality on Windows, Mac, and Linux
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef _WIN32
    // Windows
    #include <conio.h>
    #define KBHIT() _kbhit()
    #define GETCH() _getch()
#else
    // Mac/Linux
    #include <unistd.h>
    #include <termios.h>
    #include <sys/select.h>

    inline int KBHIT() {
        struct timeval tv = {0, 0};
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
    }

    inline int GETCH() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

#endif // PLATFORM_H
