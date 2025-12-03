#include "util.h"

static inline uint64_t max_num(uint8_t digits, const char *buf, uint32_t start, uint32_t bufend)
{
    uint64_t v = 0;
    for (uint32_t l = 0, m = 0, mi = 0; l < digits; ++l, m = 0, mi = 0)
    {
        for (uint32_t k = start; k < bufend - (digits - l - 1); ++k)
        {
            if (buf[k] > m)
            {
                m = buf[k];
                mi = k;
            }
        }
        v = v * 10 + (m - '0');
        start = mi + 1;
    }

    return v;
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint64_t t1 = 0, t2 = 0, s = 0;
    for (uint32_t i = 0; i < size; ++i)
    {
        if (f[i] == '\n')
        {
            t1 += max_num(2, f, s, i);
            t2 += max_num(12, f, s, i);
            s = i + 1;
        }
    }

    printf("%luns\n", clock() - begin);
    printf("%lu %lu\n", t1, t2);
}
