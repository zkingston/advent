#include "util.h"

#define D 99

struct Cell
{
    int8_t g, v, l, r, u, d, ll[10], lr[10], lu[10], ld[10];
};

int main(void)
{
    const char *f;
    load(&f, INPUT_FILENAME);

    struct Cell c[D + 2][D + 2] = {0}, df;
    memset(&df, 0xFF, sizeof(df));
    df.v = 0;

    for (uint8_t i = 0; i < D + 2; ++i)
        c[i][0] = c[i][D + 1] = df;

    for (uint8_t i = 0; i < D + 2; ++i)
        c[0][i] = c[D + 1][i] = df;

    for (uint8_t i = 0; i < D; ++i)
        for (uint8_t j = 0; j < D; ++j)
        {
            const struct Cell *l = &c[i][j + 1], *u = &c[i + 1][j];
            struct Cell *e = &c[i + 1][j + 1];

            e->v |= (e->g = f[i * (D + 1) + j] - '0') > MIN(l->l, u->u);
            e->l = MAX(e->g, l->l);
            e->u = MAX(e->g, u->u);

            for (uint8_t k = 0; k < 10; ++k)
            {
                e->ll[k] = l->ll[k] + 1;
                e->lu[k] = u->lu[k] + 1;
            }

            e->ll[l->g] = 1;
            e->lu[u->g] = 1;
        }

    int32_t score = 0, visible = 0;
    for (uint8_t i = 0; i < D; ++i)
        for (uint8_t j = 0; j < D; ++j)
        {
            const struct Cell *r = &c[D + 1 - i][D - j], *d = &c[D - i][D + 1 - j];
            struct Cell *e = &c[D - i][D - j];

            visible += e->v |= e->g > MIN(r->r, d->d);
            e->r = MAX(e->g, r->r);
            e->d = MAX(e->g, d->d);

            for (uint8_t k = 0; k < 10; ++k)
            {
                e->lr[k] = r->lr[k] + 1;
                e->ld[k] = d->ld[k] + 1;
            }

            e->lr[r->g] = 1;
            e->ld[d->g] = 1;

            int8_t sl = D, sr = D, su = D, sd = D;
            for (uint8_t k = e->g; k < 10; ++k)
            {
                sl = MIN(sl, e->ll[k]);
                sr = MIN(sr, e->lr[k]);
                su = MIN(su, e->lu[k]);
                sd = MIN(sd, e->ld[k]);
            }

            score = MAX(score, sl * sr * su * sd);
        }

    printf("%d\n%d\n", visible, score);
}
