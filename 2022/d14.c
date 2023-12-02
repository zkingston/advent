#include "util.h"

#define W 1000
#define H 175

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint16_t px = 0, py = 0, my = 0;
    bool o[W][H] = {0};
    for (uint32_t i = 0; i < size; ++i)
    {
        uint16_t x = 0, y = 0;
        for (; f[i] != ','; ++i)
            x = x * 10 + f[i] - '0';
        i++;

        for (; f[i] != ' ' && f[i] != '\n'; ++i)
            y = y * 10 + f[i] - '0';

        my = MAX(my, y);

        if (px && py)
        {
            for (uint16_t xi = MIN(px, x); xi <= MAX(px, x); ++xi)
                o[xi][y] = true;

            for (uint16_t yi = MIN(py, y); yi <= MAX(py, y); ++yi)
                o[x][yi] = true;
        }

        if (f[i] == '\n')
            px = py = 0;
        else
        {
            px = x, py = y;
            i += 3;
        }
    }

    for (uint16_t xi = 0; xi < W; ++xi)
        o[xi][my + 2] = true;

    bool drop = false;
    uint32_t n = 0;
    for (;; ++n)
    {
        uint16_t x = 500, y = 0;
        for (;;)
        {
            if (!drop && y >= my)
            {
                printf("%d\n", n);
                drop = true;
            }
            else if (!o[x][y + 1])
                y++;
            else if (!o[x - 1][y + 1])
                x--, y++;
            else if (!o[x + 1][y + 1])
                x++, y++;
            else
            {
                if (x == 500 && y == 0)
                {
                    printf("%d\n", n + 1);
                    printf("%luns\n", clock() - begin);
                    return 0;
                }

                o[x][y] = true;
                break;
            }
        }
    }
}
