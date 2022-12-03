#include "util.h"

#define B(i) (1U << (f[i] - 'a'))

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint16_t fi = 4;
    uint32_t set = B(0) ^ B(1) ^ B(2) ^ B(3);
    for (; fi < size && __builtin_popcount(set) != 4; ++fi)
        set ^= B(fi) ^ B(fi - 4);

    uint16_t gi = 14;
    set = B(0) ^ B(1) ^ B(2) ^ B(3) ^ B(4) ^ B(5) ^ B(6) ^ B(7) ^ B(8) ^ B(9) ^ B(10) ^ B(11) ^ B(12) ^ B(13);
    for (; gi < size && __builtin_popcount(set) != 14; ++gi)
        set ^= B(gi) ^ B(gi - 14);

    printf("%d\n%d\n", fi, gi);
}
