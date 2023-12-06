#include "util.h"

struct Interval
{
    uint64_t start;
    uint64_t range;
};

struct MapInterval
{
    uint64_t dst;
    struct Interval src;
};

#define IN_MI(i, n) (i.src.start <= n && n < (i.src.start + i.src.range))
#define REMAP_MI(i, n) i.dst + (n - i.src.start)

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);
    uint32_t i = 7;

    uint64_t seeds[100] = {0};
    uint8_t ns = 0;

    struct Interval ranges[1000] = {0};
    uint16_t nr = 0;

    for (; f[i] != '\n'; ++i)
        if (f[i] == ' ')
            ns++;
        else
            seeds[ns] = seeds[ns] * 10 + (f[i] - '0');

    nr = ns / 2 + 1;
    for (uint8_t k = 0; k < nr; k++)
        ranges[k] = (struct Interval){seeds[2 * k], seeds[2 * k + 1]};

    i += 20;
    uint8_t skips[7] = {25, 26, 21, 27, 30, 27, 0};
    for (uint8_t n = 0; n < 7; i += skips[n], ++n)
    {
        uint64_t intervals[100][3] = {0};
        uint8_t ni = 0;
        const struct MapInterval *mis = (struct MapInterval *)intervals;

        for (; f[i] != '\n' && i < size; ++i, ++ni)
            for (uint8_t j = 0; f[i] != '\n'; ++i)
                if (f[i] == ' ')
                    j++;
                else
                    intervals[ni][j] = intervals[ni][j] * 10 + (f[i] - '0');

        for (uint8_t j = 0; j <= ns; ++j)
            for (uint8_t k = 0; k < ni; ++k)
                if (IN_MI(mis[k], seeds[j]))
                {
                    seeds[j] = REMAP_MI(mis[k], seeds[j]);
                    break;
                }

        struct Interval new_ranges[1000] = {0};
        uint16_t new_nr = 0;

        for (uint16_t j = 0, remapped = 0; j < nr; ++j)
        {
            const struct Interval r = ranges[j];
            for (uint8_t k = 0; k < ni; ++k)
            {
                const struct MapInterval m = mis[k];
                const uint64_t er = r.start + r.range - 1, em = m.src.start + m.src.range - 1;
                const bool r_start_in = IN_MI(m, r.start), r_end_in = IN_MI(m, er);

                if (!(remapped = (r_start_in || r_end_in)))
                    continue;

                if (r_start_in && r_end_in)
                    new_ranges[new_nr++] = (struct Interval){REMAP_MI(m, r.start), r.range};

                else if (r_start_in)
                {
                    const uint64_t new_range = em - r.start + 1;
                    new_ranges[new_nr++] = (struct Interval){REMAP_MI(m, r.start), new_range};
                    ranges[nr++] = (struct Interval){r.start + new_range, r.range - new_range};
                }
                else if (r_end_in)
                {
                    const uint64_t new_range = m.src.start - r.start;
                    new_ranges[new_nr++] = (struct Interval){m.dst, r.range - new_range};
                    ranges[nr++] = (struct Interval){r.start, new_range};
                }

                break;
            }

            if (!remapped)
                new_ranges[new_nr++] = r;
        }

        memcpy(ranges, new_ranges, sizeof(struct Interval) * new_nr);
        nr = new_nr;
    }

    uint64_t min = seeds[0], min2 = ranges[0].start;
    for (uint8_t k = 1; k <= ns; ++k)
        min = MIN(min, seeds[k]);

    for (uint8_t k = 1; k < nr; ++k)
        min2 = MIN(ranges[k].start, min2);

    printf("%lu\n%lu\n", min, min2);
}
