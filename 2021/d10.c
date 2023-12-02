#include "util.h"

char pair[255] = {0};
uint32_t err_s[255] = {0};
uint8_t com_s[255] = {0};

static inline int cmp(const uint64_t *p1, const uint64_t *p2)
{
    return (*p1 < *p2) ? -1 : ((*p1 > *p2) ? 1 : 0);
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    pair['('] = ')';
    pair['['] = ']';
    pair['{'] = '}';
    pair['<'] = '>';

    err_s[')'] = 3;
    err_s[']'] = 57;
    err_s['}'] = 1197;
    err_s['>'] = 25137;

    com_s['('] = 1;
    com_s['['] = 2;
    com_s['{'] = 3;
    com_s['<'] = 4;

    uint32_t total = 0;
    uint64_t acs[255];
    uint8_t acsp = 0;

    char stack[255] = {0};
    uint8_t sp = 1;
    for (uint32_t i = 0; i < size; ++i, ++sp)
    {
        const char c = f[i];
        stack[sp] = c;

        if (c == '\n')
        {
            uint64_t ac = 0;
            for (; sp > 0; --sp)
                ac = 5 * ac + com_s[(uint8_t)stack[sp]];

            if (ac != 0)
                acs[acsp++] = ac;

            sp = 0;
        }
        else if (pair[(uint8_t)stack[sp]] == 0)
        {
            if (pair[(uint8_t)stack[sp - 1]] != stack[sp])
            {
                total += err_s[(uint8_t)c];
                for (; f[i] != '\n'; ++i)
                    ;
                sp = 0;
            }
            else
                sp -= 2;
        }
    }

    qsort(acs, acsp, sizeof(uint64_t), (int (*)(const void *, const void *))cmp);

    printf("%u\n", total);
    printf("%lu\n", acs[acsp / 2]);
}
