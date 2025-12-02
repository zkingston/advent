#ifndef ADVENT
#define ADVENT

#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>

static inline uint32_t load(const char **f, const char *filename)
{
    const int fd = open(filename, O_RDONLY);

    struct stat s;
    fstat(fd, &s);

    const uint32_t size = (uint32_t)s.st_size;
    *f = (const char *)mmap(0, (size_t)s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    return size;
}

static inline void set_no_ICANON()
{
    static struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

#define MAX(i, j) ((i > j) ? i : j)
#define MIN(i, j) ((i > j) ? j : i)
#define SGN(x) ((0 < x) - (x < 0))

#define SWAP(T, x, y)                                                                                        \
    {                                                                                                        \
        T temp = x;                                                                                          \
        x = y;                                                                                               \
        y = temp;                                                                                            \
    }

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define CRESET "\e[0m"

static inline uint8_t count_digits(uint64_t n)
{
    if (n < 10)
        return 1;
    if (n < 100)
        return 2;
    if (n < 1000)
        return 3;
    if (n < 10000)
        return 4;
    if (n < 100000)
        return 5;
    if (n < 1000000)
        return 6;
    if (n < 10000000)
        return 7;
    if (n < 100000000)
        return 8;
    if (n < 1000000000)
        return 9;
    if (n < 10000000000ULL)
        return 10;
    if (n < 100000000000ULL)
        return 11;
    if (n < 1000000000000ULL)
        return 12;
    if (n < 10000000000000ULL)
        return 13;
    if (n < 100000000000000ULL)
        return 14;
    if (n < 1000000000000000ULL)
        return 15;
    return 16;
}

static const uint64_t POW10[] = {1,
                                 10,
                                 100,
                                 1000,
                                 10000,
                                 100000,
                                 1000000,
                                 10000000,
                                 100000000,
                                 1000000000,
                                 10000000000ULL,
                                 100000000000ULL,
                                 1000000000000ULL,
                                 10000000000000ULL,
                                 100000000000000ULL,
                                 1000000000000000ULL,
                                 10000000000000000ULL};

#endif
