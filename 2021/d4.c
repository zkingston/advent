#include "util.h"

#define TOTAL 100
#define RS (15)
#define BS (RS * 5 + 1)
#define M(i) ((__uint128_t)1 << i)
#define C(a, b) ((a & b) == a)

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint32_t i = 0;
    uint8_t b = 1;
    uint8_t play[TOTAL + 1] = {0}, values[TOTAL + 1][5][5];
    __uint128_t mask[TOTAL + 1] = {0};
    for (uint8_t total = 1; i < size; ++i)
    {
        const char c = f[i];
        if (c == '\n')
            break;
        else if (c == ',')
        {
            mask[total] = mask[total - 1] | M(play[total]);
            total++;
        }
        else
            play[total] = play[total] * 10 + c - '0';
    }

    i += 2;

    uint8_t l = UINT8_MAX, m = 0, lb = 0, mb = 0;
    for (; i < size; i += BS, ++b)
    {
        __uint128_t rows[5] = {0}, cols[5] = {0};
        for (uint8_t j = 0; j < 5; ++j)
            for (uint8_t k = 0; k < 5; ++k)
            {
                const char c1 = f[i + (j * RS) + k * 3];
                const char c2 = f[i + (j * RS) + k * 3 + 1];
                values[b][j][k] = ((c1 != ' ') ? (c1 - '0') * 10 : 0) + c2 - '0';

                rows[j] |= M(values[b][j][k]);
                cols[k] |= M(values[b][j][k]);
            }

        for (uint8_t t = 1; t < TOTAL; ++t)
        {
            uint8_t v = 0;
            for (uint8_t k = 0; k < 5; ++k)
                v += C(rows[k], mask[t]) || C(cols[k], mask[t]);

            if (v)
            {
                if (t < l)
                {
                    l = t;
                    lb = b;
                }
                else if (t > m)
                {
                    m = t;
                    mb = b;
                }

                break;
            }
        }
    }

    uint16_t gl = 0, gm = 0;
    for (uint8_t j = 0; j < 5; ++j)
        for (uint8_t k = 0; k < 5; ++k)
        {
            gl += !C(M(values[lb][j][k]), mask[l]) * values[lb][j][k];
            gm += !C(M(values[mb][j][k]), mask[m]) * values[mb][j][k];
        }

    printf("%d\n%d\n", gl * play[l], gm * play[m]);
}
