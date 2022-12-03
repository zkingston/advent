#include "util.h"

#define WS 3

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint16_t index = WS;
    uint16_t prior[WS];
    for (int i = 0; i < WS; ++i)
        prior[i] = UINT16_MAX;

    uint16_t v = 0, inc1 = 0, inc3 = 0;
    for (uint32_t i = 0; i < size; ++i)
    {
        const char c = f[i];
        if (c == '\n')
        {
            inc1 += v > prior[(index - 1) % WS];
            inc3 += v > prior[(index - WS) % WS];
            prior[index++ % WS] = v;
            v = 0;
        }
        else
            v = v * 10 + c - '0';
    }

    printf("%d\n%d\n", inc1, inc3);
}
