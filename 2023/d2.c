#include "util.h"

#define M_R 12
#define M_G 13
#define M_B 14

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    bool valid = true;
    uint8_t game = 1;
    uint32_t total = 0, power = 0;
    for (uint32_t i = 0; i < size; ++i)
    {
        valid = true;
        for (; f[i] != ':'; ++i)
            ;

        i += 2;

        uint8_t r = 0, g = 0, b = 0, mr = 0, mg = 0, mb = 0;
        for (; i < size; ++i)
        {
            uint8_t v = 0;
            for (; '0' <= f[i] && f[i] <= '9'; ++i)
                v = v * 10 + f[i] - '0';

            const char c = f[(i += 1)];
            if (c == 'r')
            {
                r = v;
                i += 3;
            }
            else if (c == 'g')
            {
                g = v;
                i += 5;
            }
            else if (c == 'b')
            {
                b = v;
                i += 4;
            }

            if (f[i] == ',')
                i++;

            else
            {
                valid = r <= M_R && g <= M_G && b <= M_B;

                mr = MAX(mr, r);
                mg = MAX(mg, g);
                mb = MAX(mb, b);

                r = g = b = 0;

                if (f[i] == '\n')
                {
                    power += mr * mg * mb;
                    total += (valid) ? game++ : 0;
                    break;
                }

                i++;
            }
        }
    }

    printf("%u\n%u\n", total, power);
}
