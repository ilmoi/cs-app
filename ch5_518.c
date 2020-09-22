#include <stdio.h>
#include <math.h>

typedef double data_t;
typedef data_t (*func) (data_t[], data_t, long);

data_t poly(data_t a[], data_t x, long degree) {
    long i;
    data_t result = a[0];
    data_t xpwr = x; /* Equals x^i at start of loop */
    for (i = 1; i <= degree; i++) {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    printf("%s", __func__);
    return result;
}

data_t horner(data_t a[], data_t x, long degree) {
    long i;
    data_t result = a[degree];
    for (i=degree-1; i>=0; i--) {
        result = a[i] + x*result;
    }
    printf("%s", __func__);
    return result;
}

data_t unrolled_horner(data_t a[], data_t x, long degree) {
    long i;
    data_t result = a[degree];

    long k = 5;
    long limit = 0+k-1;

    //loop unrolling
    for (i=degree-1; i>=limit; i-=k) {
        result = a[i-4] + x*(
                a[i-3] + x*(
                a[i-2] + x*(
                a[i-1] + x*(
                a[i] + x*result))));
    }

    //tail, continuing from where we left off with i
    for (; i>=0; i--) {
        result = a[i] + x * result;
    }
    printf("%s", __func__);
    return result;
}

data_t unrolled_poly(data_t a[], data_t x, long degree) {
    long i;
    data_t result = a[0];
    data_t xpwr = x; /* Equals x^i at start of loop */

    long k = 5;
    long limit = degree-k+1;

    for (i = 1; i <= limit; i+=k) {
        result = result
                + a[i] * xpwr
                + a[i+1] * xpwr * pow(x, 1)
                + a[i+2] * xpwr * pow(x, 2)
                + a[i+3] * xpwr * pow(x, 3)
                + a[i+4] * xpwr * pow(x, 4);
        xpwr = xpwr * pow(x, 5);
    }

    //tail
    for (; i <= degree; i++) {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    printf("%s", __func__);
    return result;
}

data_t unrolled_parallel_poly(data_t a[], data_t x, long degree) {
    long i;
    data_t result = a[0];
    data_t xpwr = x; /* Equals x^i at start of loop */

    long k = 5;
    long limit = degree-k+1;

    for (i = 1; i <= limit; i+=k) {
        //adding parallelization
        data_t r1 = a[i] * xpwr;
        data_t r2 = a[i+1] * xpwr * pow(x, 1);
        data_t r3 = a[i+2] * xpwr * pow(x, 2);
        data_t r4 = a[i+3] * xpwr * pow(x, 3);
        data_t r5 = a[i+4] * xpwr * pow(x, 4);

        result += r1;
        data_t t1 = r2+r3;
        data_t t2 = r4+r5;
        t1 += t2;
        result += t1;

        xpwr = xpwr * pow(x, k);
    }

    //tail
    for (; i <= degree; i++) {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    printf("%s", __func__);
    return result;
}

data_t unrolled_parallel_poly2(data_t a[], data_t x, long degree) {
    long i;
    data_t result = a[0];
    data_t result2 = 0;
    data_t result3 = 0;
    data_t xpwr = x; /* Equals x^i at start of loop */
    data_t xpwr2 = pow(x,2);
    data_t xpwr3 = pow(x,4);

    long k = 5;
    long limit = degree-k+1;

    for (i = 1; i <= limit; i+=k) {
        //adding parallelization
        result += a[i] * xpwr;
        result2 += a[i+1] * xpwr2 + a[i+2] * xpwr2*x;
        result3 += a[i+3] * xpwr3 + a[i+4] * xpwr3*x;

        xpwr *= pow(x, k);
        xpwr2 *= pow(x, k);
        xpwr3 *= pow(x, k);
    }

    //tail
    for (; i <= degree; i++) {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }
    printf("%s", __func__);
    return result;
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
    long n = 999999;
    data_t a[n];
    for (int i = 1; i<=n; i++) {
        a[i-1] = i/10;
    }
    data_t x = 1;

    func func_array[6] = {
            &poly, //third
            &horner, //fifth
            &unrolled_poly, //2nd
            &unrolled_horner, //fourth
            &unrolled_parallel_poly, //2nd fastest
            &unrolled_parallel_poly2 //fastest by a magnitude
    };

    //interesting - so poly actually works quicker than horner even though horner requires fewer ops

    for (int i = 0; i < 6; i++) {
        unsigned long cpu_count1 = rdtsc();
        data_t r = func_array[i](a, x, n-1);
        unsigned long cpu_count2 = rdtsc();
        printf(": %f\n", r);
        printf("cpu count: %u\n", cpu_count2-cpu_count1);
    }
}