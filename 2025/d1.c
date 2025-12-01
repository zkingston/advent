#include "util.h"

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    int16_t l = 1, d = 50, v = 0, z = 0, s = 0;
    for (uint32_t i = 0; i < size; ++i)
    {
        const char c = f[i];
        if (c == '\n')
        {
            s += ((100 + d * l) % 100 + v) / 100;
            z += !(d = (d + l * v) % 100);
            v = 0;
        }
        else if (c == 'L')
            l = -1;
        else if (c == 'R')
            l = 1;
        else
            v = v * 10 + c - '0';
    }

    printf("%luns\n", clock() - begin);
    printf("%d %d\n", z, s);
}
