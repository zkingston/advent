#include "util.h"

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint32_t total = 0, power = 0;
    for (uint32_t i = 0, game = 1; i < size; ++i, ++game)
    {
        for (; f[i] != ':'; ++i)
            ;

        i += 2;

        uint8_t r = 0, g = 0, b = 0;
        for (; i < size; i += 2)
        {
            uint8_t v = 0;
            for (; '0' <= f[i] && f[i] <= '9'; ++i)
                v = v * 10 + f[i] - '0';

            const char c = f[(i += 1)];
            if (c == 'r')
            {
                r = MAX(r, v);
                i += 3;
            }
            else if (c == 'g')
            {
                g = MAX(g, v);
                i += 5;
            }
            else if (c == 'b')
            {
                b = MAX(b, v);
                i += 4;
            }

            if (f[i] == '\n')
            {
                power += r * g * b;
                total += (r <= 12 && g <= 13 && b <= 14) ? game : 0;
                break;
            }
        }
    }

    printf("%u\n%u\n", total, power);
}
