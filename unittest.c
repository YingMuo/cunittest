#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

#include "unittest.h"

#define BLU "\e[0;34m"
#define RED "\e[0;31m"
#define CRESET "\e[0m"

int signum[] = {
    SIGSEGV,
    SIGFPE,
    SIGABRT,
    SIGUSR1
};

jmp_buf cunit_env;
const char *cunit_err_msg;
int cunit_test_ctr;
int cunit_wrong_ctr;

void sig_handler(int sig)
{
    siglongjmp(cunit_env, WRONG);
}

void cunit_wrong()
{
    ++cunit_wrong_ctr;
    printf("%02d: ", cunit_test_ctr);
    printf( RED "error" CRESET ": ");
    printf(cunit_err_msg);
    printf("\n");
}

int cunit_run(CUnit *cunit)
{
    struct sigaction act = {0};
    act.sa_handler = &sig_handler;
    for (int i = 0; i < sizeof(signum) / sizeof(int); ++i)
    {
        if (sigaction(signum[i], &act, NULL) == -1)
            return -1;
    }

    for (int i = 0; cunit[i].test; ++i)
    {
        printf(BLU "Test" CRESET " %s: \n", cunit[i].name);
        cunit_test_ctr = 0;
        cunit_wrong_ctr = 0;
        cunit[i].test((void *)&cunit[i]);
        if (!cunit_wrong_ctr)
            printf("OK\n");
        printf("\n");
    }

    act.sa_handler = SIG_DFL;
    for (int i = 0; i < sizeof(signum) / sizeof(int); ++i)
    {
        if (sigaction(signum[i], &act, NULL) == -1)
            return -1;
    }

    return 0;
}