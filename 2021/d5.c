#include "util.h"

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint16_t x = 0, ht = 0, dt = 0;
    int16_t line[4] = {0};
    uint8_t horz[1000][1000] = {0}, diag[1000][1000] = {0};
    for (uint32_t i = 0; i < size; ++i)
    {
        const char c = f[i];
        if (c == '\n')
        {
            if (line[0] == line[2] || line[1] == line[3])
                for (uint16_t j = MIN(line[0], line[2]); j <= MAX(line[0], line[2]); ++j)
                    for (uint16_t k = MIN(line[1], line[3]); k <= MAX(line[1], line[3]); ++k)
                    {
                        horz[j][k]++;
                        diag[j][k]++;
                    }

            else
            {
                const int16_t hi = (line[0] < line[2]) ? 1 : -1;
                const int16_t vi = (line[1] < line[3]) ? 1 : -1;
                for (uint16_t j = 0; j <= abs(line[0] - line[2]); ++j)
                    diag[line[0] + hi * j][line[1] + vi * j]++;
            }

            bzero(line, sizeof(int16_t) * 4);
            x = 0;
        }
        else if (c == ',')
            x++;

        else if (c == ' ')
        {
            x++;
            i += 3;
        }
        else
            line[x] = line[x] * 10 + c - '0';
    }

    for (uint16_t i = 0; i < 1000; ++i)
        for (uint16_t j = 0; j < 1000; ++j)
        {
            ht += horz[i][j] > 1;
            dt += diag[i][j] > 1;
        }

    printf("%d\n%d\n", ht, dt);
}

/*/
const int o1 = line[0] < line[2];
const int o2 = line[1] < line[3];

if (o1 && o2)
    for (int j = 0; j <= line[2] - line[0]; ++j)
        diag[line[0] + j][line[1] + j]++;

else if (!o1 && o2)
    for (int j = 0; j <= line[0] - line[2]; ++j)
        diag[line[2] + j][line[1] + j]++;

else if (o1 && !o2)
    for (int j = 0; j <= line[2] - line[0]; ++j)
        diag[line[0] + j][line[3] + j]++;

else
    for (int j = 0; j <= line[0] - line[2]; ++j)
        diag[line[2] + j][line[3] + j]++;
*/
