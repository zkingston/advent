#include "util.h"

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint64_t r[2] = {0}, j = 0, t1 = 0, t2 = 0;
    for (uint32_t i = 0; i < size; ++i)
    {
        const char c = f[i];
        if (c == ',' || c == '\n')
        {
            for (uint64_t k = r[0]; k <= r[1]; ++k)
            {
                uint8_t d = floor(log10(k)) + 1;
                for (uint8_t s = 2; s <= 7; ++s)
                {
                    if (d % s == 0)
                    {
                        bool y = true;
                        uint64_t kc = k, m = pow(10, d / s);
                        for (uint8_t w = 0; w < s - 1 && y; ++w)
                        {
                            y &= kc % m == (kc / m) % m;
                            kc /= m;
                        }

                        if (y)
                        {
                            t1 += (s == 2) * k;
                            t2 += k;
                            break;
                        }
                    }
                }
            }

            r[0] = 0, r[1] = 0, j = 0;
        }
        else if (c == '-')
            j++;
        else
            r[j] = r[j] * 10 + c - '0';
    }

    printf("%luns\n", clock() - begin);
    printf("%lu %lu\n", t1, t2);
}
