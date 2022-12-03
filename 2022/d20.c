#include "util.h"

#define M 5000
#define KEY 811589153
#define SHIFT(a, o, n) memmove((a) + (o), (a), (n) * (sizeof(a)))

static inline int64_t mix(const int64_t file[], uint16_t n, uint8_t times)
{
    int64_t indices[n];
    for (uint16_t i = 0; i < n; ++i)
        indices[i] = i;

    for (uint8_t t = 0; t < times; ++t)
        for (uint16_t i = 0; i < n; ++i)
        {
            int64_t j = 0;
            for (; indices[j] != i && j < n; ++j)
                ;

            SHIFT(indices + j + 1, -1, n - j);          // pop
            const int64_t r = (j + file[i]) % (n - 1);  // shift
            const int64_t k = (r < 0) ? r + (n - 1) : r;
            SHIFT(indices + k, 1, n - k - 1);  // push
            indices[k] = i;
        }

    int64_t j = 0;
    for (; file[indices[j]] && j < n; ++j)
        ;

    return file[indices[(j + 1000) % n]] + file[indices[(j + 2000) % n]] + file[indices[(j + 3000) % n]];
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    int64_t file[M] = {0}, encr[M];
    uint16_t n = 0;
    for (uint32_t i = 0; i < size; ++i, ++n)
    {
        bool negative = f[i] == '-';
        i += negative;

        for (; f[i] != '\n'; ++i)
            file[n] = file[n] * 10 + f[i] - '0';

        encr[n] = KEY * (file[n] = (1 - 2 * negative) * file[n]);
    }

    printf("%ld\n%ld\n", mix(file, n, 1), mix(encr, n, 10));
}
