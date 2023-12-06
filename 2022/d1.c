#include "util.h"

#define N_MAX 3

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint16_t v = 0;
    uint32_t max[N_MAX + 1] = {0};
    for (uint32_t i = 0; i < size; ++i)
    {
        const char c = f[i];
        if (c == '\n')
        {
            if (v == 0)
            {
                max[2] = MAX(max[2], max[3]);
                max[1] = MAX(max[1], max[2]);
                max[0] = MAX(max[0], max[1]);
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

    printf("%luns\n", clock() - begin);
    printf("%d\n%d\n", max[0], sum);
}
