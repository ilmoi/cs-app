#include <stdio.h>
#include <assert.h>
#include <limits.h>

//create byte pointer type
typedef unsigned char* byte_pointer;

//takes a pointer for some value start, and its length
void show_bytes(byte_pointer start, size_t len) {
  size_t i;

  //iterates over bytes from start till start+len, printing 1 byte at a time in hex format
  for (i = 0; i < len; i++) {
    printf(" %.2x", start[i]);
  }
  printf("\n");
}

void show_int(int x) {
  //  shows bytes of first int (4 bytes)
  show_bytes((byte_pointer) &x, sizeof(int));
}

void show_short_int(short int x) {
    show_bytes((byte_pointer) &x, sizeof(short int));
}

void show_long_int(long int x) {
    show_bytes((byte_pointer) &x, sizeof(long int));
}

void show_float(float x) {
  //  shows bytes of first float (4 bytes)
  show_bytes((byte_pointer) &x, sizeof(float));
}

void show_double(double x) {
    show_bytes((byte_pointer) &x, sizeof(double));
}

void show_pointer(void* x) {
  //  shows bytes of first void pointer (8 bytes)
  show_bytes((byte_pointer) &x, sizeof(void*));
}

void test_show_bytes(int val) {
  int ival = val;
  short int sival = ival;
  long int lival = ival;
  float fval = (float) ival;
  int* pval = &ival;

  show_int(ival);
  show_short_int(sival);
  show_long_int(lival);
  show_float(fval);
  show_pointer(pval);
}

int is_little_endian(int x) {
    //2.58
    int first_byte = (x & 0xFF);
    //printf("%d", first_byte);
    if (first_byte == 0x39) {
        return 1;
    } else {
        return 0;
    }
}

void merge_bytes() {
    //2.59
    int x = 0x89ABCDEF;
    int y = 0x76543210;
    byte_pointer px = (byte_pointer)&x;
    byte_pointer py = (byte_pointer)&y;
    size_t i;
    int combined;
    for (i=0; i<sizeof(int); i++) {
        if (i == 0) {
            combined = px[i];
        } else {
            combined = combined << 8 | py[i];
        }
    }
    printf("%x", combined);
}

void replace_bytes(int x, int b_to_replace, int new_b) {
    //2.60
    int mask = 0xff;
    mask = mask << 8 * b_to_replace;
    new_b = new_b << 8 * b_to_replace;
    int res = (x & ~mask) | new_b;
    printf("%x", res);
}

void play_w_conditions() {
    int a = 5;
    int b = 9;
    a > b ? printf("yay") : printf("%d");
}

void checking_bits(int x) {
    //261
    //check if any bit evaluates to 1
    //if x is exactly 0 then this produces 0, else produces 1
    int has_one = x && 0xff;
    //check if any bit evaluates to 0
    //if x has at least one zero, when we flip it it has at least one one, which means overall produces one
    int has_zero = ~x && 0xff;

    //check if 1 present in last byte
    byte_pointer xp = (byte_pointer)&x;
    int least_sig_byte = xp[0];
    int least_sig_has_one = least_sig_byte && 0xff;

    //check if 0 present in last byte (need to negate first, only then select last byte, else you get 0xffffff00)
    int negated_x = ~x;
    byte_pointer negxp = (byte_pointer)&negated_x;
    int negated_least_sig_byte = negxp[0];
    int least_sig_has_zero = negated_least_sig_byte && 0xff;

    int res = has_one && has_zero && least_sig_has_one && least_sig_has_zero;

    printf("has one %d\n", has_one);
    printf("has zero %d\n", has_zero);
    printf("least has one %d\n", least_sig_has_one);
    printf("least has zero %d\n", least_sig_has_zero);
    printf("total result is: %d\n", res);
}

void any_odd_one(int x) {
    //264
    byte_pointer xp = (byte_pointer)&x;
    //build a mask where every odd bit is 1 else 0
    int penultimate_bit = 2;
    int total_mask = 0;
    for (int i=0; i<16; i++) {
        total_mask = total_mask | penultimate_bit;
        total_mask = total_mask<<2;
    }
    total_mask = total_mask | penultimate_bit;

    //select only odd bits
    int res = x & total_mask;
    int has_odd_one = !!res; //by double negating we are turning some value > 0 into 1.
    printf("%d", has_odd_one);
}

void int_size_is_32_on_16_bit_machine() {
    //in C we are not allowed to left shift by more than word size (so on this machine we can't write more than <<31)
    //here assume word size is 16
    //then we have to do this:
    int should_be_ok = ((1 << 15) << 15) << 1; //31 total
    int should_not_be_ok = should_be_ok << 1;
    printf("%d\n", (should_be_ok && !should_not_be_ok));
}

int mask_with_n_lsb_set_to_one(int n) {
    //assume 1 <= n <= w

    //my way - doesn't dynamically take into account word size and has to use if for 32 bits
    //int res;
    //if (n == 32) {
    //    printf("%x", 0xffffffff);
    //} else {
    //    res = (1<<n)-1;
    //    printf("%x", res);
    //}

    //their way
    //first calc int size
    int w = sizeof(int) << 3;
    //then do reverse shifting
    int all_ones = (unsigned) -1;
    int res = all_ones >> (w-n);
    printf("%x", res);
    return res;
}

int mask_with_n_msb_set_to_one(int n) {
    //first calc int size
    int w = sizeof(int) << 3;
    //then do reverse shifting
    int all_ones = (unsigned) -1;
    int res = all_ones << (w-n);
    printf("msb: %x\n", res);
    return res;
}


void rotating_shift(int n) {
    //269
    int w = sizeof(int) << 3;
    int x = 0x12345678;
    int mask = mask_with_n_msb_set_to_one(n);
    int temp = x & mask;
    temp = temp >> (w-n);
    x = x << n;
    int res = x | temp;
    printf("res is %x\n", res);
}


void fits_bits(int x, int n) {
    //1 = does, 0 = doesn't

    //this approach works except for n=32.
    //int res = (x >> (n-1)) >> 1;

    //here we shift the boi left then right and if it fits then it never overflows
    int w = sizeof(int) << 3;
    int offset = w-n;
    int res = (x << offset >> offset) == x;
    printf("%d", res);
}


typedef unsigned packed_t;

void xbyte(packed_t word, int bytenum) {
    int size = sizeof(packed_t);
    int shift_left = (size -1 - bytenum) << 3;
    int shift_right = (size -1) << 3;
    int ret = (int)word << shift_left >> shift_right;
    printf("%x", ret);
}

void saturating_add(int x, int y) {
    //instead of overflowing into either end returns Tmax or Tmin
    //calculate the sum - we don't yet know if it overflows or not.
    int sum = x+y;

    //we're going to use this to select the most significant bit
    int sig_mask = INT_MIN;
    /*
    * if x > 0, y > 0 but sum < 0, it's a positive overflow
    * if x < 0, y < 0 but sum >= 0, it's a negetive overflow
    */
    //remember we can't even use <>, so we have to go this creative route
    //remember that first bit 1 = negative number, first bit 0 = positive number
    int pos_overflow_happened = !(x & sig_mask) && !(y & sig_mask) && (sum & sig_mask);
    int neg_overflow_happened = (x & sig_mask) && (y & sig_mask) && !(sum & sig_mask);

    //now here's another trick we have to use
    pos_overflow_happened && (sum = INT_MAX) || neg_overflow_happened && (sum = INT_MIN);
    printf("%d", sum);
}


void tsub_ok(int x, int y) {
    int diff = x-y;
    int sig_mask = INT_MIN;
    //overflow during diff happens when
    // x>0, y<0, then can overflow to <0, eg 7 -(-8) = 15, but becomes -7
    int pos_overflow_happened = !(x & sig_mask) && (y & sig_mask) && (diff & sig_mask);
    int neg_overflow_happened = (x & sig_mask) && !(y & sig_mask) && !(diff & sig_mask);

    //now here's another trick we have to use
    int overflowed = pos_overflow_happened || neg_overflow_happened;
    printf("%d", overflowed);
}

void multiply_tester() {
    int i = 1;
    int a = (i<<4) + i;
    int b = i - (i<<3);
    int c = (i<<6) - (i<<2);
    int d = (i<<4) - (i<<7);
    printf("%d, %d, %d, %d",a,b,c,d);
}

int divide_power2(int x, int k) {
    int temp = (x>>(k-1));
    int res;
    int round_up = temp & 1 == 1;
    temp = temp >> 1;
    round_up && (res = temp+1) || (res = temp);

    //int ret;
    //if (temp & 1 == 1) {
    //    ret = (temp>>1) + 1;
    //} else {
    //    ret = temp>>1;
    //}
    //printf("%d", res);
    return res;
}

int divide_power2_towards0(int x, int k) {
    int is_negative = !!(x & INT_MIN);
    printf("is neg %d\n", is_negative);
    int temp = (x>>(k-1));
    int res;
    int needs_rounding = temp & 1 == 1;
    temp = temp >> 1;
    is_negative && needs_rounding && (res = temp+1) || needs_rounding && (res = temp-1) || (res = temp);
    return res;
}

int divide_power2_directed_towards0(int x, int k, int dir) {
    printf("%d\n", dir);
    //1 = positive direction, 0 = negative
    int temp = (x>>(k-1));
    int needs_rounding = temp & 1 == 1;
    temp = temp >> 1;
    int res = temp;
    !dir && needs_rounding && (res +=1); //only need to adjust the negative in this end
    return res;
}

void mul3div4(int x) {
    int res_of_mult = (x<<1) + x;
    int res = divide_power2(res_of_mult, 2);
    printf("%d", res);
}


void threefourths(int x) {
    //this approach doesn't work. The idea was to manually add back overflown parts, but it won't work since it could have overflown multiple times and I have no way of knowing
    ////so kinda like the above but should not overflow
    ////1 detect overflow
    //int res_of_mult = (x<<1) + x;
    //int sig_mask = INT_MIN;
    ////positive
    //int positive_overflow = !(x & sig_mask) && res_of_mult <= x;
    ////negative
    //int negative_overflow = (x & sig_mask) && res_of_mult >= x;
    ////2 if overflow - divide by 4 then add int_max/4
    //int res = divide_power2(res_of_mult, 2);
    //positive_overflow && (res = divide_power2(res_of_mult, 2) + divide_power2(INT_MAX, 2)) ||
    //negative_overflow && (res = divide_power2(res_of_mult, 2) - divide_power2(INT_MAX, 2));
    //printf("%d", res);

    //better approach - split x into f (first 30 bits) and l (last 2 bits)
    //f will divide by 4 evenly, so /4*3
    //l may not, so *3/4
    //then add back together
    int f = x & ~0x03;
    int l = x & 0x03;
    int div_f = divide_power2(f,2 );
    int res_f = (div_f << 1) + div_f;
    int mult_l = (l << 1) + l;
    int is_negative = !!(x & INT_MIN);
    //the trick was that if we're dividing a negative number we needed to add a bias
    int res_l = divide_power2_directed_towards0(mult_l,2, !is_negative);
    int res = res_f + res_l;
    printf("%d", res);
}


void bit_pattern1(int k) {
    //remember -1 == all ones, so you can use that instead of 0xffffffff
    int res = -1 << (k-1) << 1;
    printf("%d", res);
}


void bit_pattern2(int k, int j) {
    int ones = (1<<k)-1;
    int res = ones << (j-1) << 1;
    printf("%x", res);
}

void test_unsigned(){
    unsigned int x = 5;
    unsigned int y = 10;
    unsigned int res = x-y;
    int res2 = (int)y-(int)x;
    res2 = - (unsigned) res2;
    printf("%u", res);
}

typedef unsigned float_bits;

float_bits float_negate(float_bits f) {
    unsigned sign = f>>31;
    unsigned exp = f>>23 & 0xff;
    unsigned frac = f & 0x7fffff;

    if (exp == 0 && frac != 0) {
        return f;
    }
    float_bits ret = (~sign)<<31 | exp<<23 | frac;
    printf("%x\n", ret);
    return ret;
}


float_bits float_absval(float_bits f) {
    unsigned sign = f>>31;
    unsigned exp = f>>23 & 0xff;
    unsigned frac = f & 0x7fffff;

    if (exp == 0 && frac != 0) {
        return f;
    }
    float_bits ret = 0 | exp<<23 | frac;
    printf("%x\n", ret);
    return ret;
}


int main(int argc, char* argv[]) {
  int test_num = 12345;
  //test_show_bytes(test_num);
  //assert(is_little_endian(test_num));
  //merge_bytes();
  //replace_bytes(0x12345678, 0, 0xAB);
  //play_w_conditions();
  //checking_bits(0x1234ff);
  //any_odd_one(0x55);
  //int_size_is_32_on_16_bit_machine();
  //mask_with_n_lsb_set_to_one(32);
  //rotating_shift(20);
  //fits_bits(0xffffffff, 32);
  //xbyte(0x123456ff, 0);
  //saturating_add(INT_MAX, INT_MAX);
  //tsub_ok(INT_MIN, INT_MAX);
  //multiply_tester();
  //divide_power2(-7, 2);
  //mul3div4(INT_MAX);
  //threefourths(-10);
  //bit_pattern2(1, 1);
  //test_unsigned();
  float_negate((unsigned)INT_MAX);
  return 0;
}

/*
 Learnings:
 1 - I'm on a little endian machine, which I know since LEAST significant bytes are printed first
 2 - int / short int / long int only differ in padding if number fits into 2 bytes, otherwise we see rounding happening on shorter types
 3 - exactly like they taught us 12345 is 0x3039 in int but 0x4640e400 in float
 4 - interesting that the double number is completely different.
*/
