#include "util.h"

#define NUM 0
#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4

struct Monkey
{
    int64_t n, a, b;
    uint8_t op;
};

const uint32_t root = 't' << 24 | 'o' << 16 | 'o' << 8 | 'r', humn = 'n' << 24 | 'm' << 16 | 'u' << 8 | 'h';

struct Monkey monkeys[2000] = {0};
uint16_t n = 0;

static inline uint16_t find(uint32_t name)
{
    for (uint16_t i = 0; i < n; ++i)
        if (monkeys[i].n == name)
            return i;
    return 0;
}

static inline int64_t math(uint16_t i)
{
    struct Monkey *m = &monkeys[i];
    if (m->op == NUM)
        return m->a;
    else if (m->op == ADD)
        return math(find(m->a)) + math(find(m->b));
    else if (m->op == SUB)
        return math(find(m->a)) - math(find(m->b));
    else if (m->op == MUL)
        return math(find(m->a)) * math(find(m->b));
    else
        return math(find(m->a)) / math(find(m->b));
}

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    for (uint32_t i = 0; i < size; ++n)
    {
        struct Monkey *m = &monkeys[n];
        m->n = *(uint32_t *)(f + i);
        i += 6;

        if ('0' <= f[i] && f[i] <= '9')
        {
            for (; f[i] != '\n'; ++i)
                m->a = m->a * 10 + f[i] - '0';
            m->op = NUM;
            ++i;
        }
        else
        {
            m->a = *(uint32_t *)(f + i);
            i += 5;
            m->op = (f[i] == '+') ? ADD : ((f[i] == '-') ? SUB : ((f[i] == '*') ? MUL : DIV));
            i += 2;
            m->b = *(uint32_t *)(f + i);
            i += 5;
        }
    }

    uint16_t r = find(root), h = find(humn);
    printf("%ld\n", math(r));

    monkeys[r].op = SUB;
    monkeys[h].a = 0;

    bool prior = false;
    int64_t inc = math(r), m;
    while ((m = math(r)))  // world's worst search
        if (m < 0)
        {
            monkeys[h].a -= inc;
            if (prior)
                inc /= 10;
        }
        else if ((prior = m > 0))
            monkeys[h].a += inc;

    printf("%lu\n", monkeys[h].a);
}
