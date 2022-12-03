#include "util.h"

#define M 10

struct Monkey
{
    uint8_t id, op, t, f, n;
    uint32_t v, d, items[UINT8_MAX];
};

static inline int cmp(const uint64_t *p1, const uint64_t *p2)
{
    return *p2 - *p1;
}

static uint8_t mi = 0;
static uint64_t modulo = 1;

static inline uint64_t simulate(struct Monkey *m, uint32_t n, uint8_t p1)
{
    uint64_t inspected[M] = {0};
    for (uint16_t t = 0; t < n; ++t)
        for (uint8_t i = 0; i < mi; m[i].n = 0, ++i)
            for (uint8_t j = 0; j < m[i].n; ++j, ++inspected[i])
            {
                const uint64_t value = (m[i].v) ? m[i].v : m[i].items[j];
                uint64_t item = (m[i].op) ? (m[i].items[j] * value) : (m[i].items[j] + value);
                item = (p1) ? item / 3 : item % modulo;
                const uint8_t ix = (item % m[i].d == 0) ? m[i].t : m[i].f;
                m[ix].items[m[ix].n++] = item;
            }

    qsort(inspected, mi, sizeof(uint64_t), (int (*)(const void *, const void *))cmp);
    return inspected[0] * inspected[1];
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    struct Monkey m[M] = {0}, n[M];
    for (uint32_t i = 0; i < size; ++mi)
    {
        m[mi].id = f[(i += 7)] - '0';

        i += 21;
        for (;; ++i)
        {
            const char c = f[i];
            if (c == ' ')
                break;
            else if (c == '\n' || c == ',')
            {
                ++m[mi].n;
                ++i;
            }
            else
                m[mi].items[m[mi].n] = m[mi].items[m[mi].n] * 10 + f[i] - '0';
        }

        m[mi].op = f[(i += 22)] == '*';

        i += 2;
        if (f[i] != 'o')
            while (f[i] != '\n')
                m[mi].v = m[mi].v * 10 + f[i++] - '0';
        else
            i += 3;
        i += 22;

        while (f[i] != '\n')
            m[mi].d = m[mi].d * 10 + f[i++] - '0';

        modulo *= m[mi].d;

        m[mi].t = f[(i += 30)] - '0';
        m[mi].f = f[(i += 32)] - '0';
        i += 3;
    }

    memcpy(n, m, sizeof(struct Monkey) * mi);
    printf("%lu\n%lu\n", simulate(m, 20, 1), simulate(n, 10000, 0));
}
