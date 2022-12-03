#include "util.h"

#define ISO(x, y, z) (occ[x + 1][y + 1][z + 1])
#define ISF(x, y, z) (fill[x + 1][y + 1][z + 1])

bool occ[UINT8_MAX][UINT8_MAX][UINT8_MAX] = {0};
bool fill[UINT8_MAX][UINT8_MAX][UINT8_MAX] = {0};

uint8_t min[3] = {UINT8_MAX, UINT8_MAX, UINT8_MAX};
uint8_t max[3] = {0};

static inline void flood(uint8_t x, uint8_t y, uint8_t z)
{
    if (fill[x][y][z] || occ[x][y][z])
        return;

    fill[x][y][z] = true;

    if (x > 0)
        flood(x - 1, y, z);
    if (y > 0)
        flood(x, y - 1, z);
    if (z > 0)
        flood(x, y, z - 1);

    if (x < max[0] + 2)
        flood(x + 1, y, z);
    if (y < max[1] + 2)
        flood(x, y + 1, z);
    if (z < max[2] + 2)
        flood(x, y, z + 1);
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    for (uint32_t i = 0; i < size; ++i)
    {
        uint8_t c[3] = {0};
        for (uint8_t j = 0; j < 2; ++j, ++i)
            for (; f[i] != ','; ++i)
                c[j] = c[j] * 10 + f[i] - '0';
        for (; f[i] != '\n'; ++i)
            c[2] = c[2] * 10 + f[i] - '0';

        for (uint8_t j = 0; j < 3; ++j)
            min[j] = MIN(min[j], c[j]), max[j] = MAX(max[j], c[j]);

        ISO(c[0], c[1], c[2]) = true;
    }

    flood(0, 0, 0);

    uint16_t s1 = 0, s2 = 0;
    for (uint8_t i = min[0]; i <= max[0]; ++i)
        for (uint8_t j = min[1]; j <= max[1]; ++j)
            for (uint8_t k = min[2]; k <= max[2]; ++k)
                if (ISO(i, j, k))
                {
                    s1 += !ISO(i - 1, j, k) + !ISO(i + 1, j, k) + !ISO(i, j - 1, k) + !ISO(i, j + 1, k) +
                          !ISO(i, j, k - 1) + !ISO(i, j, k + 1);
                    s2 += ISF(i - 1, j, k) + ISF(i + 1, j, k) + ISF(i, j - 1, k) + ISF(i, j + 1, k) +
                          ISF(i, j, k - 1) + ISF(i, j, k + 1);
                }

    printf("%d\n%d\n", s1, s2);
}
