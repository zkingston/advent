#include "util.h"

#define EXIST 1
#define OCCUP 2
#define N 0
#define E 1
#define S 2
#define W 3

struct Step
{
    uint8_t length;
    bool turn;
};

struct Face
{
    uint8_t n, i, j, c[4], e[4], x[4];
};

struct Face p1[7] = {0}, p2[7] = {0};
struct Step steps[UINT16_MAX] = {0};
uint16_t ns = 0;
uint8_t input[UINT8_MAX][UINT8_MAX] = {0}, g;

static inline uint16_t gcd(uint16_t u, uint16_t v)
{
    return (v != 0) ? gcd(v, u % v) : u;
}

static inline void fill_dir(struct Face f[], uint8_t src, uint8_t dst, uint8_t sd, uint8_t dd, bool flip)
{
    f[src].c[sd] = dst;
    f[dst].c[dd] = src;
    f[src].e[sd] = dd;
    f[dst].e[dd] = sd;
    f[src].x[sd] = f[dst].x[dd] = flip;
}

static inline uint8_t traverse(struct Face f[], uint8_t src, uint8_t sd)
{
    static const uint8_t map[4][4] = {
        {2, 3, 0, 1},
        {1, 2, 3, 0},
        {0, 1, 2, 3},
        {3, 0, 1, 2},
    };
    return map[sd][f[src].e[sd]];
}

static inline void forward(struct Face f[], uint8_t *k, uint8_t *i, uint8_t *j, uint8_t *d)
{
    static const int8_t nav[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    uint8_t tk = *k, td = *d;
    int8_t ti = *i + nav[*d][0], tj = *j + nav[*d][1];

    if (ti < 0 || ti >= g || tj < 0 || tj >= g)
    {
        const bool flip = f[*k].x[*d];
        const uint8_t ed = f[*k].e[*d], nd = (*d + traverse(f, *k, *d)) % 4;
        ti = *i, tj = *j;

        if (*d == N && ed == E)  // welcome to hell
            ti = g - 1 - tj, tj = g - 1;
        else if (*d == N && ed == S)
            ti = g - 1, tj = (flip) ? tj : g - 1 - tj;
        else if (*d == N && ed == W)
            ti = tj, tj = 0;
        else if (*d == E && ed == N)
            tj = g - 1 - ti, ti = 0;
        else if (*d == E && ed == S)
            tj = ti, ti = g - 1;
        else if (*d == E && ed == W)
            tj = 0, ti = (flip) ? ti : g - 1 - ti;
        else if (*d == S && ed == N)
            ti = 0, tj = (flip) ? tj : g - 1 - tj;
        else if (*d == S && ed == E)
            ti = tj, tj = g - 1;
        else if (*d == S && ed == W)
            ti = g - 1 - tj, tj = 0;
        else if (*d == W && ed == N)
            tj = ti, ti = 0;
        else if (*d == W && ed == E)
            tj = g - 1, ti = (flip) ? ti : g - 1 - ti;
        else if (*d == W && ed == S)
            tj = g - 1 - ti, ti = g - 1;
        else if ((*d == E || *d == W) && *d == ed && !flip)
            ti = g - 1 - ti;
        else if ((*d == S || *d == N) && *d == ed && !flip)
            tj = g - 1 - tj;

        tk = f[*k].c[*d], td = nd;
    }

    if (input[f[tk].i * g + ti][f[tk].j * g + tj] != OCCUP)
        *i = ti, *j = tj, *k = tk, *d = td;
}

static inline void fill_missing_torus(uint8_t i, uint8_t dir)
{
    struct Face *f1 = &p1[i], *f2 = f1;
    if (!f1->c[dir])
    {
        const uint8_t d = (dir + 2) % 4;
        while (f2->c[d])
            f2 = &p1[f2->c[d]];

        fill_dir(p1, i, f2->n, dir, d, true);
    }
}

static inline void fill_missing_cube(uint8_t i, uint8_t dir)
{
    struct Face *f1 = &p2[i];
    if (!f1->c[dir])
    {
        const uint8_t od1 = (dir + 1) % 4, od2 = (dir + 3) % 4;
        const uint8_t t1 = f1->c[od1], dd1 = traverse(p2, i, od1), d1 = (dir + traverse(p2, i, od1)) % 4;
        const uint8_t o1 = p2[t1].c[d1], dd2 = dd1 + traverse(p2, t1, d1);
        if (o1)
            fill_dir(p2, i, o1, dir, (od2 + dd2) % 4, dd2 % 4 == 3);
    }
}

static inline uint32_t follow(struct Face f[])
{
    uint8_t k = 1, d = S, ci = 0, cj = 0;
    for (uint16_t i = 0; i < ns; ++i)
        if (steps[i].turn)
            d = (d + steps[i].length) % 4;
        else
            for (uint8_t j = 0; j < steps[i].length; ++j)
                forward(f, &k, &ci, &cj, &d);

    static const uint8_t score[4] = {2, 1, 0, 3};
    return 1000 * (f[k].j * g + cj + 1) + 4 * (f[k].i * g + ci + 1) + score[d];
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint32_t i = 0, w = 0, h = 0;
    for (uint16_t x = 0;; ++i, ++x)
        if (f[i] == '\n')
            if (x == 0)
                break;
            else
                w = MAX(w, x), x = -1, h++;
        else if (f[i] == '.')
            input[x][h] = EXIST;
        else if (f[i] == '#')
            input[x][h] = OCCUP;


    for (; i < size - 1; ++ns)
        if (f[i] == 'L')
            steps[ns].length = 1, steps[ns].turn = true, i++;
        else if (f[i] == 'R')
            steps[ns].length = 3, steps[ns].turn = true, i++;
        else
            for (; f[i] != 'L' && f[i] != 'R'; ++i)
                steps[ns].length = steps[ns].length * 10 + f[i] - '0';

    g = gcd(w, h);
    for (uint8_t fi = 1, j = 0; j < h / g; ++j)
        for (uint8_t i = 0; i < w / g; ++i)
            if (input[g * i][g * j])
                p1[fi].n = fi, p1[fi].i = i, p1[fi].j = j, fi++;

    for (uint8_t i = 1; i < 7; ++i)  // get adjacency in faces (i: ns j: ew)
        for (uint8_t j = 1; j < 7; ++j)
            if (p1[i].j == p1[j].j)
            {
                if (p1[i].i + 1 == p1[j].i)
                    fill_dir(p1, i, j, S, N, true);
                else if (p1[i].i - 1 == p1[j].i)
                    fill_dir(p1, i, j, N, S, true);
            }
            else if (p1[i].i == p1[j].i)
            {
                if (p1[i].j + 1 == p1[j].j)
                    fill_dir(p1, i, j, E, W, true);
                else if (p1[i].j - 1 == p1[j].j)
                    fill_dir(p1, i, j, W, E, true);
            }

    memcpy(p2, p1, sizeof(p1));

    for (uint8_t k = 0; k < 3; ++k)
        for (uint8_t i = 1; i < 7; ++i)
            for (uint8_t d = 0; d < 4; ++d)
                fill_missing_torus(i, d), fill_missing_cube(i, d);

    printf("%d\n%d\n", follow(p1), follow(p2));
}
