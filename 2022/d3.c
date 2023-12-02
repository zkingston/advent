#include "util.h"

#define TOCHR(m) ((char)__builtin_ctzll(m) + 'A')
#define SCORE(c) (((c <= 'Z') ? c - 'A' + 26 : c - 'a') + 1)

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint8_t index = 0;
    uint16_t total = 0, total3 = 0;
    uint32_t p = 0;
    uint64_t masks[3];

    for (uint32_t i = 0; i < size; i += 2)
        if (f[i] == '\n')
        {
            const uint8_t mi = index++ % 3;
            uint64_t b = masks[mi] = 0;

            const uint8_t n2 = (i - p) / 2;
            for (uint8_t j = 0; j < n2; ++j)
            {
                masks[mi] |= 1L << (f[p + j] - 'A');
                b |= 1L << (f[p + n2 + j] - 'A');
            }

            total += SCORE(TOCHR(masks[mi] & b));
            masks[mi] |= b;

            total3 += (mi == 2) * SCORE(TOCHR(masks[0] & masks[1] & masks[2]));
            p = ++i;
        }

    printf("%luns\n", clock() - begin);
    printf("%d\n%d\n", total, total3);
}
