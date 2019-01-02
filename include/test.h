
#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <assert.h>

#define test(statement, CMP) \
    do { \
        printf("%s\n", statement); \
        assert(CMP); \
    } while (0)

#endif
