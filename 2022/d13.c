#include "util.h"

#define ME 5
#define ML 128

struct Entry
{
    uint8_t l;      // is list
    uint8_t e[ME];  // index into set
    uint8_t n;
};

struct List
{
    struct Entry l[ML];
    uint8_t n;
};

const char *f;
uint32_t i = 0;
struct List ls[500] = {0};

static inline void parse_list(struct List *l, uint8_t parent)
{
    i++;
    while (f[i] != ']')
        if (f[i] == '[')
        {
            l->l[parent].e[l->l[parent].n++] = l->n;  // save child to parent
            l->l[l->n].l = 1;                         // mark child as list
            parse_list(l, l->n++);                    // recurse
        }
        else if (f[i] == ',')
            i++;
        else
        {
            l->l[parent].e[l->l[parent].n++] = l->n;

            uint8_t v = 0;
            for (; f[i] != ',' && f[i] != ']' && f[i] != '['; ++i)
                v = v * 10 + f[i] - '0';

            l->l[l->n++].n = v;
        }

    i++;
}

#define I2L(li, le)                                                                                          \
    le->l = 1;                                                                                               \
    le->e[0] = li->n;                                                                                        \
    li->l[li->n++].n = le->n;                                                                                \
    le->n = 1;

static inline int __compare_lists(struct List *a, uint8_t i, struct List *b, uint8_t j)
{
    struct Entry *ae = &a->l[i], *be = &b->l[j];
    if (ae->l && be->l)
    {
        int r;
        for (uint8_t k = 0; k < MIN(ae->n, be->n); ++k)
            if ((r = __compare_lists(a, ae->e[k], b, be->e[k])) != 0)
                return r;

        return SGN(ae->n - be->n);
    }
    else if (ae->l)
    {
        I2L(b, be)
        return __compare_lists(a, i, b, j);
    }
    else if (be->l)
    {
        I2L(a, ae)
        return __compare_lists(a, i, b, j);
    }
    else
        return SGN(ae->n - be->n);
}

static inline int compare_lists(const uint16_t *i, const uint16_t *j)
{
    struct List at, bt;
    memcpy(&at, &ls[*i], sizeof(struct List));
    memcpy(&bt, &ls[*j], sizeof(struct List));
    return __compare_lists(&at, 0, &bt, 0);
}

int main(void)
{
    const uint32_t size = load(&f, INPUT_FILENAME);

    struct List *p1 = &ls[0], *p2 = &ls[1];  // [[2]], [[6]]
    p1->l[0].l = 1, p1->l[0].e[0] = 1, p1->l[0].n = 1;
    p1->l[1].l = 1, p1->l[1].e[0] = 2, p1->l[1].n = 1;
    p1->l[2].n = 2;
    p1->n = 3;

    memcpy(p2, p1, sizeof(struct List));
    p2->l[2].n = 6;

    uint16_t sum = 0, n = 1, idx[500] = {0, 1};
    for (; i < size; ++n)
    {
        const uint16_t x = 2 + (n - 1) * 2, y = x + 1;
        struct List *l1 = &ls[x], *l2 = &ls[y];
        idx[x] = x, idx[y] = y;

        l1->l[0].l = l2->l[0].l = 1;
        l1->n = l2->n = 1;

        parse_list(l1, 0);
        i++;

        parse_list(l2, 0);
        i += 2;

        sum += (compare_lists(&x, &y) == -1) ? n : 0;
    }

    qsort(idx, 2 + (n - 1) * 2, sizeof(uint16_t), (int (*)(const void *, const void *))compare_lists);

    uint16_t a = 0, b = 0;
    for (uint16_t i = 0; i < 2 + (n - 1) * 2 && (!a || !b); ++i)
        a = (idx[i] == 0) ? i : a, b = (idx[i] == 1) ? i : b;

    printf("%d\n%d\n", sum, (a + 1) * (b + 1));
}
