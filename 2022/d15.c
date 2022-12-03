#include "util.h"

#define DY 2000000
#define MX 4000000

struct Sensor
{
    int32_t x, y, d;
};

const char *f;
uint32_t i = 12;

static inline void read_int(int32_t *v, char stop)
{
    uint8_t negative = f[i] == '-';
    i += negative;

    for (; f[i] != stop; ++i)
        *v = *v * 10 + f[i] - '0';

    *v *= 1 - 2 * negative;
}

int main(void)
{
    const uint32_t size = load(&f, INPUT_FILENAME);

    struct Sensor sensors[UINT8_MAX] = {0};
    uint8_t n = 0;

    for (; i < size; i += 13)
    {
        struct Sensor *s = &sensors[n++];
        read_int(&s->x, ',');
        i += 4;
        read_int(&s->y, ':');
        i += 25;

        int32_t x = 0, y = 0;
        read_int(&x, ',');
        i += 4;
        read_int(&y, '\n');

        s->d = abs(s->x - x) + abs(s->y - y);
    }

    int32_t max = 0, min = INT32_MAX;
    for (uint8_t i = 0; i < n; ++i)
    {
        struct Sensor *s = &sensors[i];
        int32_t d = abs(s->y - DY);
        if (d <= s->d)
        {
            max = MAX(s->x + (s->d - d), max);
            min = MIN(s->x - (s->d - d), min);
        }
    }
    printf("%d\n", max - min);

    /**
     *   /\  <- neg lines  check intersection of neg and pos +1,
     *  /  \               as will be on boundary of all sensors
     *  \  /
     *   \/  <- pos lines
     */
    int32_t pos[n * 2], neg[n * 2];
    for (uint8_t i = 0; i < n; ++i)
    {
        struct Sensor *s = &sensors[i];
        pos[i * 2] = -s->x + s->y + s->d + 1;
        pos[i * 2 + 1] = -s->x + s->y - s->d - 1;
        neg[i * 2] = s->x + s->y + s->d + 1;
        neg[i * 2 + 1] = s->x + s->y - s->d - 1;
    }

    for (uint8_t i = 0; i < n * 2; ++i)
        for (uint8_t j = 0; j < n * 2; ++j)
        {
            int32_t x = (neg[j] - pos[i]) / 2, y = (pos[i] + neg[j]) / 2;
            if (x < 0 || x > MX || y < 0 || y > MX)
                continue;

            bool v = true;
            for (uint8_t k = 0; k < n && v; ++k)
            {
                struct Sensor *s = &sensors[k];
                v &= (abs(s->x - x) + abs(s->y - y)) > s->d;
            }

            if (v)
            {
                printf("%ld\n", MX * (int64_t)x + (int64_t)y);
                return 0;
            }
        }
}
