#include "util.h"

#define W 1000
#define H 1000
#define STX (W / 2)
#define STY (H / 2)
#define N 10

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint8_t visited2[W][H] = {0}, visitedN[W][H] = {0};
    visited2[STX][STY] = visitedN[STX][STY] = 1;

    uint16_t rope[N][2], total2 = 1, totalN = 1;

    for (uint8_t i = 0; i < N; ++i)
    {
        rope[i][0] = STX;
        rope[i][1] = STY;
    }

    for (uint32_t i = 0; i < size; ++i)
    {
        const char m = f[i++];

        uint8_t v = 0;
        while (f[++i] != '\n')
            v = v * 10 + f[i] - '0';

        for (uint8_t j = 0; j < v; ++j)
        {
            rope[0][0] += (m == 'R') ? 1 : ((m == 'L') ? -1 : 0);
            rope[0][1] += (m == 'U') ? 1 : ((m == 'D') ? -1 : 0);

            for (uint8_t k = 1; k < N; ++k)
            {
                int8_t dx = rope[k - 1][0] - rope[k][0], dy = rope[k - 1][1] - rope[k][1];

                if (abs(dx) > 1 || abs(dy) > 1)
                {
                    rope[k][0] += SGN(dx);
                    rope[k][1] += SGN(dy);
                }
            }

            total2 += (++visited2[rope[1][0]][rope[1][1]] == 1);
            totalN += (++visitedN[rope[N - 1][0]][rope[N - 1][1]] == 1);
        }
    }

    printf("%luns\n", clock() - begin);
    printf("%d\n%d\n", total2, totalN);
}
