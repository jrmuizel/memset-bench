#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif
#define SIZE 2880*1800*4*16

long long time() {
#ifdef __APPLE__
        return mach_absolute_time();
#endif
}

void fill_all(int *dst) {
        for (int i=0; i<40; i++) {
                memset(dst, 1, SIZE);
        }
}

void fill_partial(int *dst, int slices) {
        int length = SIZE;
        int slice_length = length/slices;
        for (int slice = 0; slice<slices; slice++) {
                for (int i=0; i<40; i++) {
                        memset(dst+slice*slice_length/sizeof(*dst), 1, slice_length);
                }
        }
}

void time_partial(int *dst, int slices) {
        {
        long long start = time();
        int length = SIZE;
        int slice_length = length/slices;
        for (int slice = 0; slice<slices; slice++) {
                for (int i=0; i<40; i++) {
                        memset(dst+slice*slice_length/sizeof(*dst), 1, slice_length);
                }
        }

        long long end = time();
                        mach_timebase_info_data_t timebaseInfo;
                // Apple's QA1398 suggests that the output from mach_timebase_info
                // will not change while a program is running, so it should be safe
                // to cache the result.
                kern_return_t kr = mach_timebase_info(&timebaseInfo);

                double sNsPerTick = (double)(timebaseInfo.numer) / timebaseInfo.denom;
//                printf("%f\n", sNsPerTick);
        printf("%10lld %8d %d %f\n", end - start, slices, slice_length, ((double)SIZE*40*1000*1000)/(1024.*1024*(end-start)));
        }
}

int main() {
        int *dst = malloc(SIZE); 
        fill_all(dst);
                {
                        long long start = time();
        fill_all(dst);
        long long end = time();
        printf("%lld\n", end - start);
        }
        for (int i=0; i<18; i++) {
        time_partial(dst, 1<<i);
        }
}
