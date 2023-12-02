#include "util.h"

#define H 70
#define W 41

struct Node
{
    int16_t i, j, c, d;
};

struct Node g[W][H] = {0};
struct Node *frontier[W * H] = {0};
uint16_t fi = 0, fx = 1;

static inline void expand(struct Node *m, int8_t dx, int8_t dy)
{
    const uint16_t i = m->i + dx, j = m->j + dy;
    struct Node *n = &g[i][j];
    if (!n->d && !(m->c - n->c > 1))
    {
        n->d = m->d + 1;
        frontier[fx++] = n;
    }
}

int main(void)
{
    const char *f;
    load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint8_t si = 0, sj = 0, a = 0;
    for (uint8_t i = 0; i < W; ++i)
        for (uint8_t j = 0; j < H; ++j)
        {
            const char e = f[i * (H + 1) + j];

            struct Node *n = &g[i][j];
            n->i = i, n->j = j;
            n->c = e - 'a';

            if (e == 'S')
            {
                n->c = 0;
                si = i, sj = j;
            }
            else if (e == 'E')
            {
                n->c = 25;
                n->d = 1;
                frontier[0] = n;
            }
        }

    while (fi != fx)
    {
        struct Node *n = frontier[fi++];

        if (!a && (a = n->c == 0))
            printf("%d\n", n->d - 1);

        else if (n->i == si && n->j == sj)
        {
            printf("%d\n", n->d - 1);
            printf("%luns\n", clock() - begin);
            return 0;
        }

        if (n->i > 0)
            expand(n, -1, 0);

        if (n->i < (W - 1))
            expand(n, 1, 0);

        if (n->j > 0)
            expand(n, 0, -1);

        if (n->j < (H - 1))
            expand(n, 0, 1);
    }
}
