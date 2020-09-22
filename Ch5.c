#include <stdio.h>
#include <stdlib.h>
#include "combine.h"
#include <limits.h>

/* $begin vec */
/* Create vector of specified length */
vec_ptr new_vec(long len)
{
    /* Allocate header structure */
    vec_ptr result = (vec_ptr) malloc(sizeof(vec_rec));
    data_t *data = NULL;
    if (!result)
        return NULL;  /* Couldn't allocate storage */
    result->len = len;
/* $end vec */
    /* We don't show this in the book */
    result->allocated_len = len;
/* $begin vec */
    /* Allocate array */
    if (len > 0) {
        data = (data_t *)calloc(len, sizeof(data_t));
	if (!data) {
	    free((void *) result);
 	    return NULL; /* Couldn't allocate storage */
	}
    }
    /* data will either be NULL or allocated array */
    result->data = data;
    return result;
}

/* Free storage used by vector */
void free_vec(vec_ptr v) {
    if (v->data)
	free(v->data);
    free(v);
}

/*
 * Retrieve vector element and store at dest.
 * Return 0 (out of bounds) or 1 (successful)
 */
int get_vec_element(vec_ptr v, long index, data_t *dest)
{
    if (index < 0 || index >= v->len)
	return 0;
    *dest = v->data[index];
    return 1;
}

/* Return length of vector */
long vec_length(vec_ptr v)
{
    return v->len;
}
/* $end vec */


/* $begin get_vec_start */
data_t *get_vec_start(vec_ptr v)
{
    return v->data;
}
/* $end get_vec_start */


/*
 * Set vector element.
 * Return 0 (out of bounds) or 1 (successful)
 */
int set_vec_element(vec_ptr v, long index, data_t val)
{
    if (index < 0 || index >= v->len)
	return 0;
    v->data[index] = val;
    return 1;
}


/* Set vector length.  If >= allocated length, will reallocate */
void set_vec_length(vec_ptr v, long newlen)
{
    if (newlen > v->allocated_len) {
	free(v->data);
	v->data = calloc(newlen, sizeof(data_t));
	v->allocated_len = newlen;
    }
    v->len = newlen;
}


void inner4(vec_ptr u, vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;

    for (i = 0; i < length; i++) {
        sum = sum + udata[i] * vdata[i];
    }

    *dest = sum;
}


void inner5(vec_ptr u, vec_ptr v, data_t *dest) {
    //this time with loop unrolling
    long i;
    long length = vec_length(u);
    data_t *udata = get_vec_start(u);
    data_t *vdata = get_vec_start(v);
    data_t sum = (data_t) 0;

    long k = 2;
    long limit = length - k + 1;

    for (i = 0; i < limit; i+=k) {
        sum = sum + (udata[i] * vdata[i] + (udata[i+1] * vdata[i+1]));
    }

    for (i = limit; i < length; i++) {
        sum = sum + udata[i] * vdata[i];
    }

    *dest = sum;
}


int main() {
    //create vectors
    vec_ptr u = new_vec(5);
    vec_ptr v = new_vec(5);

    //check vectors created correctly
    printf("%d\n", u->len);
    printf("%d\n", v->len);

    //set the values and check they set ok
    int i;
    for (i=0; i<5; i++) {
        set_vec_element(u, i, i+1);
        set_vec_element(v, i, i+1);
        printf("%d\n", u->data[i]);
        printf("%d\n", v->data[i]);
    }

    //inner product plus check it worked
    data_t dest;
    data_t *dest_ptr = &dest;
    inner5(u, v, dest_ptr);
    printf("%d", dest);
}