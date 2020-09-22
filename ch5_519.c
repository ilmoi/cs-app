#include <stdio.h>

typedef float (*func) (float[], float[], long);

void psum1a(float a[], float p[], long n) {
    long i;
    float last_val, val;
    last_val = p[0] = a[0];
    for (i=1; i<n; i++) {
        val = last_val + a[i];
        p[i] = val;
        last_val = val;
    }
}

void psum2(float a[], float p[], long n) {
    long i;
    float val;
    val = p[0] = a[0];
    for (i=0; i<n; i++) {
        val += a[i];
        p[i] = val;
    }
}

void psum3(float a[], float p[], long n) {
    //rewrite with loop unrolling and reassociation
    //4-way = 5 additions
    long i;
    float val = p[0] = a[0];

    long limit = n - 4 + 1;

    //loop unrolling
    for (i=1; i<limit; i+=4) {
        val += a[i];
        p[i] = val;
        val += a[i+1];
        p[i+1] = val;
        val += a[i+2];
        p[i+2] = val;
        val += a[i+3];
        p[i+3] = val;
        val += a[i+4];
        p[i+4] = val;
    }

    //tail
    for (; i<n; i++) {
        val += a[i];
        p[i] = val;
    }
}

void psum_4_1a(float a[], float p[], long n) {
    long i;
    float val, last_val;
    float tmp, tmp1, tmp2, tmp3;
    last_val = p[0] = a[0];

    for (i = 1; i < n - 4; i++) {
        tmp = last_val + a[i];
        tmp1 = tmp + a[i + 1];
        tmp2 = tmp1 + a[i + 2];
        tmp3 = tmp2 + a[i + 3];

        p[i] = tmp;
        p[i + 1] = tmp1;
        p[i + 2] = tmp2;
        p[i + 3] = tmp3;

        /* key point */
        last_val = last_val + (a[i] + a[i + 1] + a[i + 2] + a[i + 3]);
    }

    for (; i < n; i++) {
        last_val += a[i];
        p[i] = last_val;
    }
}

//------------------------------------------------------------------------------
//measuring clock cycles
unsigned long rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long)hi << 32) | lo;
}

// -----------------------------------------------------------------------------

int main() {
    int n = 6;
    float a[] = {1.1, 1.1, 1.1, 1.1, 1.1, 1.1};
    float p[] = {0, 0, 0, 0, 0, 0};
    psum3(a, p, n);
    for (int i=0; i<n; i++) {
        printf("%f\n", p[i]);
    }

    func func_array[4] = {
            &psum1a,
            &psum2,
            &psum3,
            &psum_4_1a,
    };

    for (int i = 0; i < 4; i++) {
        unsigned long cpu_count1 = rdtsc();
        func_array[i](a, p, n);
        unsigned long cpu_count2 = rdtsc();
        printf("cpu count: %u\n", cpu_count2-cpu_count1);
    }
}

//interesting turns out the 2nd one is fastest - probably because has the fewest ops