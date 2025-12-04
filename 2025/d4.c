#include "util.h"

/* #define MAX_X 10 */
#define MAX_X 139

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    bool a1[MAX_X + 2][MAX_X + 2] = {0}, a2[MAX_X + 2][MAX_X + 2] = {0};

    for (int16_t x = 0; x < MAX_X; ++x)
        for (int16_t y = 0; y < MAX_X; ++y)
        {
            const char c = f[x * (MAX_X + 1) + y];
            a1[x + 1][y + 1] = c == '@';
        }

    uint32_t total = 0;
    for (uint16_t k = 0, changed = true; changed; k++)
    {
        changed = false;
        for (int16_t x = 0; x < MAX_X; ++x)
            for (int16_t y = 0; y < MAX_X; ++y)
            {
                int8_t n = -1;
                if (a1[x + 1][y + 1])
                {
                    for (int8_t i = -1; i < 2; ++i)
                        for (int8_t j = -1; j < 2; ++j)
                            n += a1[x + 1 + i][y + 1 + j];

                    total += n < 4;
                    changed |= n < 4;
                }

                a2[x + 1][y + 1] = a1[x + 1][y + 1] && !(n < 4);
            }

        memcpy(a1, a2, sizeof(a1));
        if (!k)
            printf("%lu ", total);
    }

    printf("%lu\n", total);
    printf("%luns\n", clock() - begin);
}
