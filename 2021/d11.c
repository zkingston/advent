#include "util.h"

int main(void)
{
    const char *f;
    load(&f, INPUT_FILENAME);

    uint8_t grid[10][10];
    bool flashed[10][10] = {0};

    for (uint8_t i = 0; i < 10; ++i)
        for (uint8_t j = 0; j < 10; ++j)
            grid[i][j] = f[i * 11 + j] - '0';

    uint16_t flash = 0;
    for (uint16_t k = 0;; ++k)
    {
        bzero(flashed, sizeof(flashed));

        for (uint8_t i = 0; i < 10; ++i)
            for (uint8_t j = 0; j < 10; ++j)
                grid[i][j]++;

        for (bool flipped = true; flipped;)
        {
            flipped = false;
            for (int8_t i = 0; i < 10; ++i)
                for (int8_t j = 0; j < 10; ++j)
                    if (!flashed[i][j] && grid[i][j] > 9)
                    {
                        flipped = flashed[i][j] = true;
                        for (int8_t ii = -1; ii < 2; ++ii)
                            for (int8_t jj = -1; jj < 2; ++jj)
                            {
                                const int8_t ix = i + ii;
                                const int8_t jx = j + jj;
                                grid[ix][jx] += (ii || jj) && 0 <= ix && ix <= 9 && 0 <= jx && jx <= 9;
                            }
                    }
        }

        bool all = true;
        for (int8_t i = 0; i < 10; ++i)
            for (int8_t j = 0; j < 10; ++j)
                if (flashed[i][j])
                {
                    flash++;
                    grid[i][j] = 0;
                }
                else
                    all = false;

        if (k == 99)
            printf("%u\n", flash);

        if (all)
        {
            printf("%u\n", k + 1);
            break;
        }
    }
}
