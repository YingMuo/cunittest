#ifndef _UNITTEST_H
#define _UNITTEST_H

#include <setjmp.h>
#include <signal.h>

typedef void (*Func ) (void *data);

typedef struct cunit
{
    char *name;
    Func test;
    Func hook;
    void *hook_data;
} CUnit;

typedef enum event {
    TEST = 0,
    WRONG
} Event;

extern jmp_buf cunit_env;
extern const char *cunit_err_msg;
extern int cunit_test_ctr;

int cunit_run(CUnit *cunit);

#define CUNIT(_test, _hook, _data) \
    {.name = #_test, .test = _test, .hook = _hook, .hook_data = _data}

#define CUNIT_ASSERT_HOOK(x) \
    do \
    { \
        cunit_err_msg = "CUNIT_ASSERT_HOOK(" #x ")"; \
        switch (sigsetjmp(cunit_env, 1)) \
        { \
        case TEST: \
            ++cunit_test_ctr; \
            if (!(x)) \
                raise(SIGUSR1); \
            break; \
        case WRONG: \
            cunit_wrong(); \
            ((CUnit *)data)->hook(((CUnit *)data)->hook_data); \
            break; \
        default: \
            break; \
        } \
    } while (0)

#endif /* _UNITTEST_H */