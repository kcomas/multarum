
#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <assert.h>

#define test(statement, left, CMP, right) \
    do { \
        printf("%s\n", statement); \
        assert(left CMP right); \
    } while (0)

#endif
