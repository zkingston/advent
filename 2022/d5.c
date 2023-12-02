#include "util.h"

#define IW 9
#define IH 8
#define BX(j, k) (IW * 4 * k + 1 + (j * 4))
#define D(i) (f[i] - '0')

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint8_t grid1[IW][UINT8_MAX], grid2[IW][UINT8_MAX], x1[IW] = {0}, x2[IW] = {0};
    for (uint8_t k = 0; k < IH; ++k)
        for (uint8_t j = 0; j < IW; ++j)
            if ((grid1[j][IH - k] = grid2[j][IH - k] = f[BX(j, k)]) != ' ')
                x1[j] = ++x2[j];

    for (uint32_t i = BX(IW, IH); i < size;)
    {
        i += 5;
        const uint8_t two = f[i + 1] == ' ';
        const uint8_t n = (two) ? D(i) : (D(i) * 10 + D(i + 1));
        i += 7 + (1 - two);
        const uint8_t from = D(i);
        const uint8_t to = D(i + 5);
        i += 7;

        for (uint8_t j = 0; j < n; ++j)
            grid1[to - 1][++x1[to - 1]] = grid1[from - 1][x1[from - 1]--];

        x2[from - 1] -= n;
        memcpy(&grid2[to - 1][x2[to - 1] + 1], &grid2[from - 1][x2[from - 1] + 1], n);
        x2[to - 1] += n;
    }

    printf("%luns\n", clock() - begin);

    for (uint8_t j = 0; j < IW; ++j)
        putchar(grid1[j][x1[j]]);
    putchar('\n');

    for (uint8_t j = 0; j < IW; ++j)
        putchar(grid2[j][x2[j]]);
    putchar('\n');
}
