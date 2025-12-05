#include "util.h"

int compare(const void *a, const void *b)
{
    const uint64_t *pa = (const uint64_t *)a;
    const uint64_t *pb = (const uint64_t *)b;
    return (*pa > *pb) - (*pa < *pb);
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint64_t i = 0, r[1000][2] = {0}, j = 0, k = 0, t1 = 0, t2 = 0;
    for (; i < size; ++i)
    {
        const char c = f[i];
        if (c == '\n')
        {
            if (j == 0)
                break;
            else
            {
                k++;
                j = 0;
            }
        }
        else if (c == '-')
            j++;
        else
            r[k][j] = r[k][j] * 10 + c - '0';
    }

    qsort(r, k, sizeof(r[0]), compare);

    for (uint64_t j = 0, b = 0; j < k; b = j++)
    {
        for (; j < k && r[j][0] >= r[b][0] && r[j][0] <= r[b][1]; ++j)
            r[b][1] = MAX(r[j][1], r[b][1]);

        t2 += r[b][1] - r[b][0] + 1;
    }

    uint64_t v = 0;
    for (; i < size; ++i)
    {
        const char c = f[i];
        if (c == '\n')
        {
            for (uint64_t j = 0; j < k; ++j)
                if (v >= r[j][0] && v <= r[j][1])
                {
                    t1++;
                    break;
                }

            v = 0;
        }
        else
            v = v * 10 + c - '0';
    }

    printf("%luns\n", clock() - begin);
    printf("%lu %lu\n", t1, t2);
}
