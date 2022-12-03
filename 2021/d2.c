#include "util.h"

#define FS sizeof("forward")
#define DS sizeof("down")
#define US sizeof("up")
#define NS sizeof("X")

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint32_t v1 = 0, h1 = 0;
    uint32_t v2 = 0, h2 = 0, aim = 0;
    for (uint32_t i = 0; i < size; i += NS)
    {
        const char c = f[i];
        if (c == 'f')
        {
            const uint32_t v = f[(i += FS)] - '0';
            h1 += v;
            h2 += v;
            v2 += aim * v;
        }
        else if (c == 'd')
        {
            const uint32_t v = f[(i += DS)] - '0';
            v1 += v;
            aim += v;
        }
        else if (c == 'u')
        {
            const uint32_t v = f[(i += US)] - '0';
            v1 -= v;
            aim -= v;
        }
    }

    printf("%d\n%d\n", v1 * h1, v2 * h2);
}
