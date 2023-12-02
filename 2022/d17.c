#include "util.h"

#define NR 1000000000000UL
#define PIECE(a, b, c, d) (a << 24 | b << 16 | c << 8 | d)
#define LT PIECE(0x40, 0x40, 0x40, 0x40)
#define RT PIECE(0x01, 0x01, 0x01, 0x01)

const uint32_t pieces[5] = {
    PIECE(0x00, 0x00, 0x00, 0x1E),  // -
    PIECE(0x00, 0x08, 0x1C, 0x08),  // +
    PIECE(0x00, 0x04, 0x04, 0x1C),  // L (backwards)
    PIECE(0x10, 0x10, 0x10, 0x10),  // |
    PIECE(0x00, 0x00, 0x18, 0x18)   // []
};
const uint8_t heights[5] = {1, 3, 3, 4, 2};

struct Key
{
    uint64_t h, t;
    uint16_t m;
    uint8_t p;
};

struct Key keys[10000] = {0};
uint8_t tower[10000] = {0xFF, 0};
bool moves[UINT16_MAX];  // true if right
uint16_t n = 0, m = 0, b = 0;
uint64_t x = 1;

static inline void drop(uint8_t p)
{
    uint32_t piece = pieces[p];
    for (uint32_t h = x + 3;; m = (m + 1) % n, --h)
    {
        const uint32_t npiece = (moves[m]) ? (((RT & piece) == 0) ? piece >> 1 : piece) :
                                             (((LT & piece) == 0) ? piece << 1 : piece);
        piece = ((npiece & *(uint32_t *)(tower + h)) == 0) ? npiece : piece;

        if (piece & *(uint32_t *)(tower + h - 1))
        {
            *(uint32_t *)(tower + h) |= piece;
            x = MAX(x, h + heights[p]);

            if (x > 8)
                keys[b++] = (struct Key){.t = *(uint64_t *)(tower + x - 8), .p = p, .m = m, .h = x};

            return;
        }
    }
}

int main(void)
{
    const char *f;
    n = load(&f, INPUT_FILENAME) - 1;
    const clock_t begin = clock();

    for (uint16_t i = 0; i < n; ++i)
        moves[i] = f[i] == '>';

    uint8_t p = 0;
    uint64_t k = 0, ek = 0, ex = 0;
    for (; k + ek < NR; ++k, p = (p + 1) % 5, m = (m + 1) % n)
    {
        drop(p);

        if (k == 2021)
            printf("%lu\n", x - 1);
        else if (k > 2021 && !ex)
            for (uint16_t i = 0; i < b; ++i)
                if (keys[b - 1].t == keys[i].t && keys[b - 1].p == keys[i].p && keys[b - 1].m == keys[i].m)
                {
                    uint64_t dx = keys[b - 1].h - keys[i].h, dk = b - i - 1;
                    uint64_t cycles = (NR - k) / dk;
                    ek += cycles * dk;
                    ex += cycles * dx;
                    break;
                }
    }

    printf("%lu\n", ex + x - 1);
    printf("%luns\n", clock() - begin);
}
