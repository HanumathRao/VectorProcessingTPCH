#ifndef PERFTIMERS_H
#define PERFTIMERS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <mach/mach_time.h>
#include <stdio.h>      /* printf */
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <math.h>       /* sqrt */
#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <immintrin.h>

using namespace std;



uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}



/* Helpful conversion constants. */
static const unsigned usec_per_sec = 1000000;
static const unsigned usec_per_msec = 1000;

/* These functions are written to match the win32
 signatures and behavior as closely as possible.
 */
bool QueryPerformanceFrequency(int64_t *frequency)
{
    /* Sanity check. */
    assert(frequency != NULL);

    /* gettimeofday reports to microsecond accuracy. */
    *frequency = usec_per_sec;

    return true;
}

bool QueryPerformanceCounter(int64_t *performance_count)
{
    struct timeval time;

    /* Sanity check. */
    assert(performance_count != NULL);

    /* Grab the current time. */
    gettimeofday(&time, NULL);
    *performance_count = time.tv_usec + /* Microseconds. */
    time.tv_sec * usec_per_sec; /* Seconds. */

    return true;
}

class Performancetimer
{
public:
    Performancetimer(string testName)
    {
        m_testName = testName;
        m_clock_t = clock();
        m_rdtscCycles = rdtsc();
    }

    void GetReport()
    {
        m_rdtscCycles =rdtsc() -m_rdtscCycles;
        m_clock_t = clock() - m_clock_t;
        printf("\n");
        printf("%s RDTSC  %llu \n",m_testName.c_str(), m_rdtscCycles/1000);
        printf("%s Cycles  %lu \n",m_testName.c_str(), m_clock_t/1000);
        printf("\n");
    }

private:
    uint64_t m_rdtscCycles;
    uint64_t m_cyclesFilter;
    clock_t m_clock_t;
    string m_testName;
};

#endif // PERFTIMERS_H
