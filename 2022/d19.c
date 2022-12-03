#include "util.h"

uint8_t bp[UINT8_MAX][4][3] = {0};
uint8_t n = 1;

struct State
{
    uint8_t r[4];  // o c b g robots
    uint8_t n[4];  // o c b g resources
};

#define M 500
typedef struct State Buffer[5 * M];
struct State start = {{1, 0}, {0}};

static inline int cmp(const struct State *p1, const struct State *p2)
{
    for (uint8_t i = 0; i < 4; ++i)  // what will exist next + prod
    {
        const int c = (p2->n[3 - i] + p2->r[3 - i]) - (p1->n[3 - i] + p1->r[3 - i]);
        if (c != 0)
            return c;
    }

    return 0;
}

static inline void propagate(const uint8_t bp[4][3], struct State *s, Buffer *w, uint32_t *n)
{
    struct State next = *s;
    for (uint8_t i = 0; i < 4; ++i)  // add resources
        next.n[i] += next.r[i];

    for (uint8_t i = 0; i < 4; ++i)                                             // check each robot
        if (s->n[0] >= bp[i][0] && s->n[1] >= bp[i][1] && s->n[2] >= bp[i][2])  // robot can be built
        {
            struct State copy = next;
            for (uint8_t j = 0; j < 3; ++j)  // sub resources
                copy.n[j] -= bp[i][j];

            copy.r[i]++;
            (*w)[(*n)++] = copy;
        }

    (*w)[(*n)++] = next;  // waitin'
}

static inline uint16_t simulate(const uint8_t bp[4][3], uint8_t time)
{
    Buffer q1 = {start}, q2, *r = &q1, *w = &q2;
    uint32_t n1 = 1, n2 = 0;

    for (uint8_t t = 0; t < time; ++t, n2 = 0)
    {
        for (uint32_t i = 0; i < n1; ++i)
            propagate(bp, &(*r)[i], w, &n2);

        if (n2 > M)
        {
            qsort(*w, n2, sizeof(struct State), (int (*)(const void *, const void *))cmp);
            n2 = M;
        }

        SWAP(Buffer *, r, w);
        SWAP(uint32_t, n1, n2);
    }

    uint8_t m = 0;
    for (uint16_t i = 0; i < M; ++i)
        m = MAX(m, (*r)[i].n[3]);

    return m;
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    bool two;
    for (uint32_t i = 34; i < size; ++n, i += 46 + two + (n > 9))
    {
        bp[n][0][0] = f[i] - '0';
        i += 29;
        bp[n][1][0] = f[i] - '0';
        i += 33;
        bp[n][2][0] = f[i] - '0';
        i += 10;
        bp[n][2][1] = ((two = f[i + 1] != ' ')) ? (f[i] - '0') * 10 + (f[i + 1] - '0') : f[i] - '0';
        i += 31 + two;
        bp[n][3][0] = f[i] - '0';
        i += 10;
        bp[n][3][2] = ((two = f[i + 1] != ' ')) ? (f[i] - '0') * 10 + (f[i + 1] - '0') : f[i] - '0';
    }

    uint16_t s = 0;
    for (uint8_t i = 1; i < n; ++i)
        s += i * simulate(bp[i], 24);

    printf("%d\n", s);
    printf("%d\n", simulate(bp[1], 32) * simulate(bp[2], 32) * simulate(bp[3], 32));
}
