#include "util.h"

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint8_t j = 0, r[4] = {0};
    uint16_t ctotal = 0, ototal = 0;
    for (uint32_t i = 0; i < size; ++i)
    {
        const char c = f[i];
        if (c == '\n')
        {
            ctotal += (r[0] <= r[2] && r[1] >= r[3]) || (r[2] <= r[0] && r[3] >= r[1]);
            ototal += (r[2] <= r[0] && r[0] <= r[3]) || (r[2] <= r[1] && r[1] <= r[3]) ||
                      (r[0] <= r[2] && r[2] <= r[1]) || (r[0] <= r[3] && r[3] <= r[1]);

            bzero(r, sizeof(uint8_t) * 4);
            j = 0;
        }
        else if (c == '-' || c == ',')
            j++;
        else
            r[j] = r[j] * 10 + c - '0';
    }

    printf("%luns\n", clock() - begin);
    printf("%d\n%d\n", ctotal, ototal);
}
