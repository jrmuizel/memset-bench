#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif
#include <time.h>
#define SIZE 2880*1800*4*16

long long get_time() {
#ifdef __APPLE__
        return mach_absolute_time();
#else
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	long long p = t.tv_nsec;
	p += t.tv_sec * 1000*1000 *1000;
	return p;
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
        long long start = get_time();
        int length = SIZE;
        int slice_length = length/slices;
        for (int slice = 0; slice<slices; slice++) {
                for (int i=0; i<40; i++) {
                        memset(dst+slice*slice_length/sizeof(*dst), 1, slice_length);
                }
        }

        long long end = get_time();
        printf("%10lld %8d %d %f\n", end - start, slices, slice_length, ((double)SIZE*40*1000*1000)/(1024.*1024*(end-start)));
        }
}

int main() {
        int *dst = malloc(SIZE); 
        fill_all(dst);
                {
                        long long start = get_time();
        fill_all(dst);
        long long end = get_time();
        printf("%lld\n", end - start);
        }
        for (int i=0; i<18; i++) {
        time_partial(dst, 1<<i);
        }
}
