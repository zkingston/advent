#include "util.h"

static const uint16_t scores[3][3] = {{3, 6, 0},  //
                                      {0, 3, 6},  //
                                      {6, 0, 3}};

static const uint16_t plays[3][3] = {{2, 0, 1},  //
                                     {0, 1, 2},  //
                                     {1, 2, 0}};

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    const clock_t begin = clock();

    uint16_t score1 = 0, score2 = 0;
    for (uint32_t i = 0; i < size; i += 4)
    {
        const uint8_t a = f[i] - 'A';
        const uint8_t b = f[i + 2] - 'X';
        score1 += scores[a][b] + b + 1;

        const uint8_t c = plays[a][b];
        score2 += scores[a][c] + c + 1;
    }

    printf("%luns\n", clock() - begin);
    printf("%d\n%d\n", score1, score2);
}
