#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>

#define da_array(name, dtype)    \
typedef struct {                 \
	dtype* items;                \
	size_t count;                \
	size_t capacity;             \
} name;

#define da_append(array, x)                                                                        \
    do {                                                                                           \
        if ((array)->count >= (array)->capacity) {                                                 \
            if ((array)->capacity == 0) (array)->capacity = 256;                                   \
            else (array)->capacity *= 2;                                                           \
            (array)->items = realloc((array)->items, (array)->capacity * sizeof(*(array)->items)); \
        }                                                                                          \
                                                                                                   \
        (array)->items[(array)->count++] = (x);                                                    \
    } while (0)

#define da_clear(array)        \
	do {                       \
		array.items = 0;       \
		array.count = 0;       \
		array.capacity = 0;    \
	} while (0)

#define da_free(array)        \
  do {                        \
	  free((array)->items);   \
  } while (0)

#endif
