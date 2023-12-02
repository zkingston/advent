#include "util.h"

#define TOTAL 70000000
#define FREE 30000000
#define SMALL 100000

#define TOEND                                                                                                \
    while (f[++i] != '\n')                                                                                   \
        ;

static inline int cmp(const uint32_t *p1, const uint32_t *p2)
{
    return *p2 - *p1;
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint32_t i = 0, stack[UINT8_MAX], dirs[UINT8_MAX];
    uint8_t sp = 0, dp = 0;

    while (i < size)
        if (f[i + 2] == 'c')  // cd
        {
            i += 5;
            if (f[i] == '.')  // pop
            {
                i += 3;
                stack[sp - 1] += dirs[dp++] = stack[sp];
                sp--;
            }
            else  // push
            {
                stack[++sp] = 0;
                TOEND
            }
        }
        else  // ls
        {
            i += 6;
            for (; i < size; ++i)
            {
                if (f[i] == 'd')  // dir
                {
                    i += 4;
                    TOEND
                }
                else if (f[i] == '$')  // done
                    break;

                else  // file
                {
                    uint32_t v = 0;
                    for (; f[i] != ' '; ++i)
                        v = v * 10 + f[i] - '0';

                    TOEND
                    stack[sp] += v;
                }
            }
        }

    for (; sp > 1; --sp)  // unpopped
        stack[sp - 1] += dirs[dp++] = stack[sp];

    qsort(dirs, dp--, sizeof(uint32_t), (int (*)(const void *, const void *))cmp);

    uint32_t sum = 0;
    for (; dirs[dp] <= FREE - (TOTAL - stack[sp]); --dp)
        if (dirs[dp] <= SMALL)
            sum += dirs[dp];

    printf("%luns\n", clock() - begin);
    printf("%d\n%d\n", sum, dirs[dp]);
}
