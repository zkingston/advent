#include "util.h"

#define INTERESTING(x) (x >= 20 && ((x - 20) % 40) == 0)

int32_t x = 1, cycle = 1, strength = 0;

static inline void crt()
{
    const int32_t column = (cycle - 1) % 40;
    if (column == 0)
        putchar('\n');
    putchar((abs(column - x) < 2) ? '#' : ' ');
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    for (uint32_t i = 0; i < size; ++i, ++cycle)
    {
        crt();

        int8_t v = 0;
        if (f[i] == 'a')
        {
            i += 5;

            bool negative = f[i] == '-';
            i += negative;

            for (; f[i] != '\n'; ++i)
                v = v * 10 + f[i] - '0';
            v *= (1 - 2 * negative);

            strength += (INTERESTING(cycle)) ? cycle * x : 0;
            cycle++;

            crt();
        }
        else
            i += 4;

        strength += (INTERESTING(cycle)) ? cycle * x : 0;
        x += v;
    }

    printf("\n%d\n", strength);
}
