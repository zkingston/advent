#include "util.h"

int main(void)
{
    const char *f;
    const uint32_t size = load(&f, INPUT_FILENAME);

    uint32_t total = 0, sum = 0;
    for (uint32_t i = 0; i < size - 1;)
    {
        uint8_t nvals[8];
        for (uint8_t j = 0; j < 8; ++j)
            nvals[j] = 0x7F;

        uint8_t count = 0, pattern = 0;
        for (uint8_t j = 0; j < 10;)
        {
            const char c = f[i++];
            if (c == ' ')
            {
                nvals[count] &= pattern;
                pattern = count = 0;
                j++;
            }
            else
            {
                pattern |= 1 << (c - 'a');
                count++;
            }
        }

        // 2 & 3 & 5 -> top mid bot
        // 6 & 9 & 0 -> top lu rb bot
        const uint8_t top = nvals[3] - nvals[2];          // 1 ^ 7 -> top
        const uint8_t mid = nvals[4] & nvals[5];          // (2 & 3 & 5) & 4 -> mid
        const uint8_t bot = nvals[5] - top - mid;         // (2 & 3 & 5) - top - mid  -> bot
        const uint8_t lup = nvals[4] - nvals[2] - mid;    // 4 - 1 - mid -> lu
        const uint8_t rbt = nvals[6] & nvals[2];          // (6 & 9 & 0) & 1 -> rb
        const uint8_t rup = nvals[2] - rbt;               // 1 - rb -> ru
        const uint8_t lbt = 0x7F - nvals[4] - top - bot;  // 8 - 4 - lu -> lb
        const uint8_t seg[10] = {
            top | lup | rup | lbt | rbt | bot, nvals[2], top | rup | mid | lbt | bot,
            top | rup | mid | rbt | bot,       nvals[4], top | lup | mid | rbt | bot,
            top | lup | mid | lbt | rbt | bot, nvals[3], 0x7F,
            top | lup | rup | mid | rbt | bot,
        };

        uint8_t index[UINT8_MAX];
        for (uint8_t j = 0; j < 10; ++j)
            index[seg[j]] = j;

        i += 2;

        uint32_t value = 0;
        pattern = count = 0;
        for (uint8_t j = 0; j < 4;)
        {
            const char c = f[i++];
            if (c == ' ' || c == '\n')
            {
                if (count == 2 || count == 4 || count == 3 || count == 7)
                    total++;

                value = value * 10 + index[pattern];
                pattern = count = 0;
                j++;
            }
            else
            {
                pattern |= 1 << (c - 'a');
                count++;
            }
        }

        sum += value;
    }

    printf("%d\n%d\n", total, sum);
}
