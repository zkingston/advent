#include "util.h"

#define M 300
#define I 100
typedef bool EArray[M][M];
typedef uint16_t AABB[4];

// could just compute if elf is on edge of prev. bb, but eh
#define UAB(a, i, j)                                                                                         \
    {                                                                                                        \
        (*a)[0] = MIN((*a)[0], i), (*a)[1] = MAX((*a)[1], i);                                                \
        (*a)[2] = MIN((*a)[2], j), (*a)[3] = MAX((*a)[3], j);                                                \
    }

const int8_t nwse[8][2] = {{1, 0},    // 0 N
                           {1, 1},    // 1 NE
                           {0, 1},    // 2 E
                           {-1, 1},   // 3 SE
                           {-1, 0},   // 4 S
                           {-1, -1},  // 5 SW
                           {0, -1},   // 6 W
                           {1, -1}};  // 7 NW

const uint8_t checks[4][3] = {{6, 5, 7},   // W (accidentally goofed the orientation, rotate above by 90)
                              {2, 1, 3},   // E
                              {4, 3, 5},   // S
                              {0, 1, 7}};  // N

static inline bool move(EArray *b, AABB *a, uint16_t i, uint16_t j, bool o[], uint8_t idx)
{
    bool any = o[checks[idx][0]] || o[checks[idx][1]] || o[checks[idx][2]];

    if (!any)
    {
        const int8_t di = nwse[checks[idx][0]][0], dj = nwse[checks[idx][0]][1];
        if ((*b)[i + di][j + dj])
        {
            (*b)[i + di][j + dj] = false;
            (*b)[i][j] = (*b)[i + 2 * di][j + 2 * dj] = true;
            UAB(a, i, j);
        }
        else
        {
            (*b)[i + di][j + dj] = true;
            UAB(a, i + di, j + dj);
        }
    }

    return !any;
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME) - 1;

    EArray elves1 = {0}, elves2 = {0};
    AABB aabb1 = {I, 0, I, 0}, aabb2 = {M, 0, M, 0};

    uint16_t x = I, y = I;
    for (uint32_t i = 0; i < size; ++i)
        if (f[i] == '\n')
            y++, x = I;
        else if (f[i] == '#')
            elves1[x++][y] = true;
        else
            x++;

    aabb1[1] = x, aabb1[3] = y;

    EArray *curr = &elves1, *next = &elves2;
    AABB *cab = &aabb1, *nab = &aabb2;

    for (uint16_t k = 0;; ++k)
    {
        bool moved = false;
        for (uint16_t j = (*cab)[2]; j <= (*cab)[3]; ++j)
            for (uint16_t i = (*cab)[0]; i <= (*cab)[1]; ++i)
                if ((*curr)[i][j])
                {
                    bool any = false, occupied[8] = {0};
                    for (uint8_t idx = 0; idx < 8; ++idx)
                        any |= (occupied[idx] = (*curr)[i + nwse[idx][0]][j + nwse[idx][1]]);

                    if (!any || !(move(next, nab, i, j, occupied, (k) % 4) ||
                                  move(next, nab, i, j, occupied, (k + 1) % 4) ||
                                  move(next, nab, i, j, occupied, (k + 2) % 4) ||
                                  move(next, nab, i, j, occupied, (k + 3) % 4)))
                    {
                        (*next)[i][j] = true;
                        UAB(nab, i, j);
                    }
                    else
                        moved = true;
                }

        if (!moved)
        {
            printf("%d\n", k + 1);
            break;
        }

        SWAP(EArray *, curr, next);
        SWAP(AABB *, cab, nab);

        bzero(next, sizeof(EArray));
        (*nab)[0] = (*nab)[2] = M;
        (*nab)[1] = (*nab)[3] = 0;

        if (k == 9)
        {
            uint32_t s = 0;
            for (uint16_t j = (*cab)[2]; j <= (*cab)[3]; ++j)
                for (uint16_t i = (*cab)[0]; i <= (*cab)[1]; ++i)
                    s += !(*curr)[i][j];

            printf("%d\n", s);
        }
    }
}
