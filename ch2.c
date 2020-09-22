#include <stdio.h>

// -----------------------------------------------------------------------------
// 2.2


#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

int converter() {
    int signed_boi = -2147483647; // -4bn + 2bn = -2bn. In other words ALL PIXELS ON.
    unsigned int unsigned_boi = 2147483648;

    printf("signed boi printed as S = %d\n", signed_boi);
    printf("signed boi printed as U = %u\n", signed_boi);

    int m = 3;

    printf("m: "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n",
    BYTE_TO_BINARY(m>>8), BYTE_TO_BINARY(m));

    unsigned int u1 = 1234;
    unsigned int u2 = 1235;
    printf("%u", u1-u2);

    return 0;
}

// -----------------------------------------------------------------------------
// 2.3

#include <math.h>

int tadd_ok(int x, int y) {
    //returns 1 if can add 2 signed arguments without overflow
    int sum = x+y;
    int neg_over = x < 0 && y < 0 && sum >= 0;
    int pos_over = x > 0 && y > 0 && sum < 0;
    printf("%d", !neg_over && !pos_over);
    return !neg_over && !pos_over;
}


// -----------------------------------------------------------------------------
int div16(int x) {
    //x>>31 generates a word with all 1s if x is negative, and all 0s if positive
    //this works coz we erase everything but the first 1 or 0, and since the shift is arithmetic, we replicate that number
    //we need bias in order to fix rounding, which for negative numbers doesn't work correctly otherwise
    int bias = (x >> 31) & 0xF;
    return (x + bias) >> 4;
}

// -----------------------------------------------------------------------------
int main() {
    //tadd_ok(pow(2, 30)-1,pow(2, 30)); //2bn
    //tadd_ok(-pow(2, 30),-pow(2, 30)); //negative allows one more -2bn
    int result = div16(-33);
    printf("%d\n", result);
}