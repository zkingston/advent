#include "util.h"

#define N_MAX 3

static inline int cmp(const uint32_t *p1, const uint32_t *p2)
{
    return *p2 - *p1;
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint16_t v = 0;
    uint32_t max[N_MAX + 1] = {0};
    for (uint32_t i = 0; i < size; ++i)
    {
        const char c = f[i];
        if (c == '\n')
        {
            if (v == 0)
            {
                qsort(max, N_MAX + 1, sizeof(uint32_t), (int (*)(const void *, const void *))cmp);
                max[N_MAX] = 0;
            }
            else
            {
                max[N_MAX] += v;
                v = 0;
            }
        }
        else
            v = v * 10 + c - '0';
    }

    uint32_t sum = 0;
    for (uint8_t i = 0; i < N_MAX; ++i)
        sum += max[i];

    printf("%d\n%d\n", max[0], sum);
}
