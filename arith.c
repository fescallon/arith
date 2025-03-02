#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

char *const HELP = "Usage: sum x1 x2 x3 ... xn with n >= 1\n";

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("%s", HELP);
        return 1;
    }
    long res = 0;
    for (int i = 1; i < argc; ++i) {
        char *endptr;

        errno = 0;
        long n = strtol(argv[i], &endptr, 10);

        if (argv[i][0] == '\0' || endptr[0] != '\0' || errno) {
            fprintf(stderr, "Could not convert argument %d.\n", i);
            return 1;
        }
        if (res > 0 && n > 0 && n > LONG_MAX - res) {
            fprintf(stderr, "Result overflows long integer.\n");
            return 1;
        } 
        if (res < 0 && n < 0 && n < LONG_MIN - res) {
            fprintf(stderr, "Result underflows long integer.\n");
            return 1;
        }

        res += n;
    }
    printf("= %ld\n", res);
    return 0;
}

