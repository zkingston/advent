#include "util.h"

#define W 100
#define H 100

#define L(i, j) (g[i][j] < g[i - 1][j])
#define R(i, j) (g[i][j] < g[i + 1][j])
#define U(i, j) (g[i][j] < g[i][j - 1])
#define D(i, j) (g[i][j] < g[i][j + 1])

static inline int cmp(const uint16_t *p1, const uint16_t *p2)
{
    return *p2 - *p1;
}

static uint16_t labels[W * H];
static inline uint16_t find(uint16_t x)
{
    uint16_t y = x;

    while (labels[y] != y)
        y = labels[y];

    while (labels[x] != x)
    {
        const uint16_t z = labels[x];
        labels[x] = y;
        x = z;
    }

    return y;
}

int main(void)
{
    const char *f;
    load(&f, INPUT_FILENAME);

    uint8_t g[H + 2][W + 2];
    uint16_t b[H + 2][W + 2] = {0};

    for (uint8_t i = 0; i < H + 2; ++i)
        g[i][0] = g[i][W + 1] = 9;

    for (uint8_t i = 0; i < W + 2; ++i)
        g[0][i] = g[H + 1][i] = 9;

    for (uint8_t i = 0; i < H; ++i)
        for (uint8_t j = 0; j < W; ++j)
            g[i + 1][j + 1] = f[i * (W + 1) + j] - '0';

    uint32_t risk = 0;
    for (uint8_t i = 1; i < H + 1; ++i)
        for (uint8_t j = 1; j < W + 1; ++j)
            risk += (L(i, j) && R(i, j) && U(i, j) && D(i, j)) * (g[i][j] + 1);

    for (uint16_t i = 0; i < W * H; ++i)
        labels[i] = i;

    // good ol Hoshen–Kopelman algorithm
    uint16_t ll = 0, sums[W * H] = {0};
    for (uint8_t i = 1; i < H + 1; ++i)
        for (uint8_t j = 1; j < W + 1; ++j)
            if (g[i][j] != 9)
            {
                uint16_t left = b[i - 1][j];
                uint16_t above = b[i][j - 1];

                if (left == 0 && above == 0)
                    sums[(b[i][j] = ++ll)]++;
                else if (left != 0 && above == 0)
                    sums[(b[i][j] = find(left))]++;
                else if (left == 0 && above != 0)
                    sums[(b[i][j] = find(above))]++;
                else
                    sums[(b[i][j] = labels[find(above)] = find(left))]++;
            }

    uint16_t totals[W * H] = {0};
    for (uint16_t i = 1; i <= ll; ++i)
        totals[labels[i]] += sums[i];

    qsort(totals, ll, sizeof(uint16_t), (int (*)(const void *, const void *))cmp);
    printf("%d\n%d\n", risk, totals[0] * totals[1] * totals[2]);
}
