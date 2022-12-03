#include "util.h"

int8_t map[UINT8_MAX] = {0};
char symbols[5] = "=-012";

int main(void)
{
    map['2'] = 2, map['1'] = 1, map['-'] = -1, map['='] = -2;

    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint64_t n = 0, t = 0;
    for (uint32_t i = 0; i < size; ++i)
        if (f[i] == '\n')
        {
            t += n;
            n = 0;
        }
        else
            n = n * 5 + map[(unsigned char)f[i]];

    uint8_t i = 0;
    char output[10];
    for (; t; ++i, t = (t + 2) / 5)
        output[i] = symbols[(t + 2) % 5];

    while (i--)
        putchar(output[i]);
    putchar('\n');
}
