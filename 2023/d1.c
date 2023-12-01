#include "util.h"

const char NUMBER[9][5] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
const uint8_t SIZE[9] = {3, 3, 5, 4, 4, 3, 5, 5, 4};

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint8_t s1 = 255, e1, s2 = 255, e2;
    uint32_t sum_1 = 0, sum_2 = 0;
    for (uint32_t i = 0; i < size; ++i)
    {
        const char c = f[i];

        if ('0' <= c && c <= '9')
        {
            const uint8_t d = c - '0';
            s1 = (s1 == 255) ? d : s1;
            s2 = (s2 == 255) ? d : s2;
            e2 = e1 = d;
        }
        else if (c == '\n')
        {
            sum_1 += s1 * 10 + e1;
            sum_2 += s2 * 10 + e2;
            s2 = s1 = 255;
        }
        else
        {
            for (uint8_t j = 0; j < 9; ++j)
            {
                if (i + SIZE[j] < size && strncmp(&f[i], NUMBER[j], SIZE[j]) == 0)
                {
                    s2 = (s2 == 255) ? j + 1 : s2;
                    e2 = j + 1;
                    break;
                }
            }
        }
    }

    printf("%d\n%d\n", sum_1, sum_2);
}
