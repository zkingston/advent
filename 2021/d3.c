#include "util.h"

#define WIDTH 12
#define FILTER 0xFFF
#define R(i) (WIDTH - i - 1)
#define I(i, j) (i * (WIDTH + 1) + j)
#define V(i, j) ((uint16_t)f[I(i, j)] - '0')

static inline int cmp(const uint16_t *p1, const uint16_t *p2)
{
    return *p1 - *p2;
}

static inline void sort(uint16_t *array, const uint16_t size)
{
    qsort(array, size, sizeof(uint16_t), (int (*)(const void *, const void *))cmp);
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const uint16_t total = size / (WIDTH + 1);

    uint16_t counts[WIDTH] = {0}, m[total], l[total];
    for (uint16_t i = 0; i < total; ++i)
    {
        m[i] = l[i] = i;
        for (uint8_t j = 0; j < WIDTH; ++j)
            counts[j] += V(i, j);
    }

    uint16_t common = counts[0] > total / 2, ctotal = total;
    for (uint8_t j = 0; j < WIDTH && ctotal > 1; ++j)
    {
        uint16_t ncount = 0, ntotal = 0;
        for (uint16_t i = 0; i < ctotal; ++i)
            if (V(m[i], j) == common)
            {
                ncount += V(m[i], j + 1);
                ntotal++;
            }
            else
                m[i] = UINT16_MAX;

        sort(m, ctotal);
        common = (ntotal == ncount * 2) ? 1 : (ncount > ntotal / 2);
        ctotal = ntotal;
    }

    common = counts[0] < total / 2, ctotal = total;
    for (uint8_t j = 0; j < WIDTH && ctotal > 1; ++j)
    {
        uint16_t ncount = 0, ntotal = 0;
        for (uint16_t i = 0; i < ctotal; ++i)
            if (V(l[i], j) == common)
            {
                ncount += V(l[i], j + 1);
                ntotal++;
            }
            else
                l[i] = UINT16_MAX;

        sort(l, ctotal);
        common = (ntotal == ncount * 2) ? 0 : (ncount < ntotal / 2);
        ctotal = ntotal;
    }

    uint16_t gamma = 0, ox = 0, co = 0;
    for (int j = 0; j < WIDTH; ++j)
    {
        gamma |= (counts[j] > total / 2) << R(j);
        ox |= V(m[0], j) << R(j);
        co |= V(l[0], j) << R(j);
    }
    const uint16_t epsilon = ~gamma & FILTER;

    printf("%d\n%d\n", gamma * epsilon, ox * co);
}
