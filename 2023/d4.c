#include "util.h"

#define NC 202
#define NW 10
#define NN 25

#define GETN                                                                                                 \
    const uint8_t a = f[n + j * 3], b = f[n + j * 3 + 1], c = ((a != ' ') ? (a - '0') : 0) * 10 + (b - '0');

int main(void)
{
    const char *f;
    load(&f, INPUT_FILENAME);

    uint32_t copies[NC + 1] = {0}, total1 = 0, total2 = 0;
    for (uint8_t i = 0; i < NC; total2 += copies[i], ++i, ++copies[i])
    {
        uint32_t n = 10 + i * (12 + NW * 3 + NN * 3);
        bool winning[100] = {0};
        for (uint8_t j = 0; j < NW; ++j)
        {
            GETN;
            winning[c] = true;
        }

        n += NW * 3 + 2;

        uint8_t tw = 0;
        for (uint8_t j = 0; j < NN; ++j)
        {
            GETN;
            if (winning[c])
                copies[i + ++tw] += copies[i];
        }

        total1 += (tw) ? 1 << (tw - 1) : 0;
    }

    printf("%u\n%u\n", total1, total2);
}
