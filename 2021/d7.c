#include "util.h"

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint16_t x = 0;
    uint16_t pos[UINT16_MAX] = {0};
    for (uint32_t i = 0; i < size; ++i)
    {
        const char c = f[i];
        if (c == ',' || c == '\n')
            x++;
        else
            pos[x] = pos[x] * 10 + c - '0';
    }

    uint32_t pf = UINT32_MAX, qf = UINT32_MAX;
    for (uint16_t i = 0; i < x; ++i)
    {
        uint32_t f = 0, q = 0;
        for (uint16_t j = 0; j < x; ++j)
        {
            const uint16_t d = MAX(pos[j], i) - MIN(pos[j], i);
            f += d;
            q += d * (d + 1) / 2;
        }

        pf = (f > pf) ? pf : f;
        qf = (q > qf) ? qf : q;
    }

    printf("%u\n%u\n", pf, qf);
}

