#include <stdio.h>
#include <stdlib.h>
#include "unittest.h"

int add(int x, int y) 
{
    return x + y;
}

void test_add(void *data)
{
    CUNIT_ASSERT_HOOK(add(1, 3) == 4);
    CUNIT_ASSERT_HOOK(add(2, 5) == 8);
    CUNIT_ASSERT_HOOK(add(3, 8) == 14);
}

int sub(int x, int y) 
{
    return x - y;
}

void test_sub(void *data)
{
    CUNIT_ASSERT_HOOK(sub(-1, 5) == -6);
    CUNIT_ASSERT_HOOK(sub(8, 7) == 1);
    CUNIT_ASSERT_HOOK(sub(9, 9) == 0);
}

int mul(int x, int y)
{
    return x * y;
}

void test_mul(void *data)
{
    CUNIT_ASSERT_HOOK(mul(-3, -1) == -3);
    CUNIT_ASSERT_HOOK(mul(-3, 3) == -9);
    CUNIT_ASSERT_HOOK(mul(5, 7) == 35);
}

int mydiv(int x, int y)
{
    return x / y;
}

void test_div(void *data)
{
    CUNIT_ASSERT_HOOK(mydiv(-3, -3) == 1);
    CUNIT_ASSERT_HOOK(mydiv(3, 8) == 0);
    CUNIT_ASSERT_HOOK(mydiv(0, 0) == 0);
}

void hook(void *data)
{
    switch (cunit_test_ctr)
    {
        case 1:
        case 2:
            *(int *)data -= 7;
            break;
        case 3:
            *(int *)data -= 6;
            break;
    }
}

int main(void)
{
    int score[4] = {20, 20, 20, 20};
    CUnit cunit[] = {
        CUNIT(test_add, hook, &score[0]),
        CUNIT(test_sub, hook, &score[1]),
        CUNIT(test_mul, hook, &score[2]),
        CUNIT(test_div, hook, &score[3]),
        {0}
    };

    if (cunit_run(cunit) == -1)
    {
        perror("cunit_run");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 4; ++i)
        printf("score[%d]: %d\n", i, score[i]);
    return 0;
}