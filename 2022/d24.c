#include "util.h"

#define N 30
#define B(i) ((__uint128_t)1 << i)

__uint128_t e[N] = {B(1), 0}, o[N] = {0}, u[N] = {0}, d[N] = {0}, l[N] = {0}, r[N] = {0}, *m[UINT8_MAX] = {0};
uint16_t h = 0, n1 = 0, n2 = 0;

static inline void step()
{
    const __uint128_t tu = u[1], td = d[h - 1];
    for (uint8_t i = 1; i < h - 1; ++i)
        u[i] = u[i + 1], d[h - i] = d[h - 1 - i];
    u[h - 1] = tu, d[1] = td;

    for (uint16_t i = 1; i < h; ++i)
    {
        const __uint128_t nl = l[i] >> 1, nr = r[i] << 1;
        l[i] = (nl & o[i]) ? nl | o[i] >> 1 : nl;
        r[i] = (nr & o[i]) ? nr | o[i] << 1 : nr;
    }

    __uint128_t et[N];
    for (uint8_t i = 0; i <= h; ++i)
        et[i] = e[i] | (e[i] << 1) | (e[i] >> 1) |  //
                ((i > 0) ? e[i - 1] : 0) | ((i < h) ? e[i + 1] : 0);

    for (uint8_t i = 0; i <= h; ++i)
        e[i] = et[i] & ~o[i] & ~l[i] & ~r[i] & ~u[i] & ~d[i];
}

int main(void)
{
    m['#'] = o, m['<'] = l, m['>'] = r, m['^'] = u, m['v'] = d;

    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    for (uint32_t i = 0, w = 0; i < size - 1; ++i, ++w)
        if (f[i] == '\n')
            w = -1, h++;
        else if (m[(uint8_t)f[i]])
            m[(uint8_t)f[i]][h] |= B(w);

    for (; !(e[h] & ~o[h]); ++n1)  // there
        step();

    bzero(e, sizeof(*e) * h);
    for (; !(e[0] & ~o[0]); ++n2)  // back
        step();

    bzero(&e[1], sizeof(*e) * h);
    for (; !(e[h] & ~o[h]); ++n2)  // and there again
        step();

    printf("%u\n%u\n", n1, n1 + n2);
}
