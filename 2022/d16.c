#include "util.h"

#define AA (((uint16_t)'A') << 8 | 'A')
#define IDX (f[i] << 8 | f[i + 1])

struct Valve
{
    uint16_t x, c[UINT8_MAX];
    uint8_t f, n;
};

struct Valve valves[UINT8_MAX] = {0};
uint16_t posv[UINT8_MAX] = {AA, 0}, all_opened = 0;  // AA is always 0
uint8_t vidx[UINT16_MAX] = {0}, dist[UINT8_MAX][UINT8_MAX], nv = 0, np = 1;

static inline uint16_t dp(uint8_t time, uint8_t pos, uint16_t opened, bool elephant)
{
    static uint16_t memo[2][31][UINT16_MAX][UINT8_MAX] = {0};
    if (memo[elephant][time][opened][pos] || !opened)
        return memo[elephant][time][opened][pos];

    uint16_t pressure = 0;
    for (uint8_t i = 1; i < np; ++i)
        if (opened & (1 << i))
        {
            const uint8_t id = vidx[posv[i]];
            if (dist[pos][id] < time)
            {
                const uint8_t nt = time - dist[pos][id] - 1;
                const uint16_t p = nt * valves[id].f + dp(nt, id, opened & ~(1 << i), elephant);
                pressure = MAX(pressure, p);
            }
        }

    const uint16_t e = (elephant) ? dp(26, vidx[AA], opened, false) : 0;
    pressure = MAX(e, pressure);
    memo[elephant][time][opened][pos] = pressure;
    return pressure;
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    for (uint32_t i = 6; i < size; i += 9)
    {
        struct Valve *a = &valves[nv];
        vidx[(a->x = IDX)] = nv++;
        i += 17;

        for (; f[i] != ';'; ++i)
            a->f = a->f * 10 + f[i] - '0';

        if (a->f > 0)
        {
            all_opened |= 1 << np;
            posv[np++] = a->x;
        }

        if (f[(i += 24)] == ' ')
        {
            i++;
            for (; f[i + 2] != '\n'; i += 4)
                a->c[a->n++] = IDX;
        }

        a->c[a->n++] = IDX;
    }

    for (uint8_t i = 0; i < nv; ++i)
        for (uint8_t j = 0; j < nv; ++j)
            dist[i][j] = UINT8_MAX;

    for (uint8_t i = 0; i < nv; ++i)
    {
        dist[i][i] = 0;
        struct Valve *a = &valves[i];
        for (uint8_t j = 0; j < a->n; ++j)
            dist[i][vidx[a->c[j]]] = 1;
    }

    for (uint8_t k = 0; k < nv; ++k)
        for (uint8_t i = 0; i < nv; ++i)
            for (uint8_t j = 0; j < nv; ++j)
                dist[i][j] = MIN(dist[i][j], dist[i][k] + dist[k][j]);

    printf("%d\n%d\n", dp(30, vidx[AA], all_opened, false), dp(26, vidx[AA], all_opened, true));
}
