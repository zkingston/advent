#include "util.h"

// #define DAYS 80
#define DAYS 256

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint64_t fish[9] = {0};
    for (uint32_t i = 0; i < size; i += 2)
        fish[f[i] - '0']++;

    uint64_t birth = 0;
    for (uint16_t d = 0; d <= DAYS; ++d)
    {
        fish[(d + 6) % 9] += fish[(d + 8) % 9] = birth;
        birth = fish[d % 9];
        fish[d % 9] = 0;
    }

    uint64_t nf = birth;
    for (uint8_t i = 0; i < 9; ++i)
        nf += fish[i];

    printf("%lu\n", nf);
}
