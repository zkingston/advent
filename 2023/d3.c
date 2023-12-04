#include "util.h"

#define S 140

int main(void)
{
    const char *f;
    load(&f, INPUT_FILENAME);

    uint16_t active[S][S] = {0}, gp = 2;
    for (int16_t i = 0; i < S; ++i)
        for (int16_t j = 0; j < S; ++j)
        {
            const char c = f[i * (S + 1) + j];
            const bool is_gear = c == '*';

            if (!('0' <= c && c <= '9') && c != '.')
                for (int16_t ii = -1; ii < 2; ++ii)
                    for (int16_t jj = -1; jj < 2; ++jj)
                    {
                        const int16_t ix = i + ii;
                        const int16_t jx = j + jj;
                        if (0 <= ix && ix < S && 0 <= jx && jx < S)
                            active[ix][jx] = (is_gear) ? gp : 1;
                    }

            gp += is_gear;
        }

    uint32_t total = 0, gear1[1000] = {0}, gear2[1000] = {0};
    for (uint8_t i = 0; i < S; ++i)
    {
        uint16_t g = 0, v = 0;
        for (uint8_t j = 0; j < S; ++j)
        {
            const char c = f[i * (S + 1) + j];
            if ('0' <= c && c <= '9')
            {
                g = MAX(g, active[i][j]);
                v = v * 10 + c - '0';
            }
            else
            {
                if (g)
                {
                    total += v;
                    if (!gear1[g])
                        gear1[g] = v;
                    else
                        gear2[g] = v;
                }

                g = v = 0;
            }
        }

        if (g)
        {
            total += v;
            if (!gear1[g])
                gear1[g] = v;
            else
                gear2[g] = v;
        }
    }

    uint32_t gs = 0;
    for (uint16_t i = 2; i < gp; ++i)
        gs += gear1[i] * gear2[i];

    printf("%u\n%u\n", total, gs);
}
