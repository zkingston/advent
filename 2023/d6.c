#include "util.h"

#define N 4
#define NP(x) ((uint64_t)((x < 10) ? 10 : ((x < 100) ? 100 : ((x < 1000) ? 1000 : 10000))))

double r(uint64_t t, uint64_t d)
{
    const double b = sqrt((double)(t * t - 4 * d));
    double r1 = 0.5 * (t + b), r2 = 0.5 * (t - b);
    r1 -= (r1 == (uint64_t)r1) ? 1 : 0;
    r2 += (r2 == (uint64_t)r2) ? 1 : 0;
    return floor(r1) - ceil(r2) + 1;
}

int main(void)
{
    const char *f;
    load(&f, INPUT_FILENAME);

    uint32_t time[N], dist[N];
    sscanf(f, "Time: %u %u %u %u\nDistance: %u %u %u %u\n",  //
           time, time + 1, time + 2, time + 3,               //
           dist, dist + 1, dist + 2, dist + 3);

    uint64_t ttime = 0, tdist = 0, p = 1;
    for (uint8_t i = 0; i < N; ++i)
        p = p * r(time[i], dist[i]), ttime = ttime * NP(time[i]) + time[i],
        tdist = tdist * NP(dist[i]) + dist[i];

    printf("%lu\n%lu\n", p, (uint64_t)r(ttime, tdist));
}
