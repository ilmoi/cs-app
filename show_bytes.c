/*
 * show-bytes.c
 */

#include <stdio.h>

//create byte pointer
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

void show_float(float x) {
  //  shows bytes of first float (4 bytes)
  show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void* x) {
  //  shows bytes of first void pointer (8 bytes)
  show_bytes((byte_pointer) &x, sizeof(void*));
}

void test_show_bytes(int val) {
  int ival = val;
  float fval = (float) ival;
  int* pval = &ival;

  show_int(ival);
  show_float(fval);
  show_pointer(pval);
}

int main(int argc, char* argv[]) {
  int test_num = 555;

  test_show_bytes(test_num);
  return 0;
}

//learnings:
//when you cast the same int as different data types, the byte representation changes