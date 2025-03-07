#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>

char *const USAGE = "Usage: arith [-MODE] x1 x2 x3 ... xn for integers xi. Type \"arith -h\" for more.\n";
char *const HELP = "Usage: arith [-MODE] x1 x2 x3 ... xn for integers xi.\n"
                    "Modes\n"
                    "-s/(empty): addition\n"
                    "-m: arithmetic mean\n";

char get_mode(int argc, char **argv) {
    int opt, ret = 'S';
    while ((opt = getopt(argc, argv, "hsm")) != -1) {
        if (ret != 'S') return -1;
        switch (opt) {
            case 'h':
            case 's':
            case 'm':
                ret = opt;
                break;
            default:
                return -1;
        }
    }
    return ret;
}

int sum_overflows(long res, long n) {
    if (res > 0 && n > 0 && n > LONG_MAX - res) return 1;
    if (res < 0 && n < 0 && n < LONG_MIN - res) return -1;
    return 0;
}


long add_args(int argc, char **argv, unsigned start) {
    if (start >= argc) return 0;
    long res = 0;
    for (int i = start; i < argc; ++i) {
        char *endptr;

        errno = 0;
        long n = strtol(argv[i], &endptr, 10);

        if (argv[i][0] == '\0' || endptr[0] != '\0' || errno) {
            fprintf(stderr, "Could not convert argument %d.\n", i);
            exit(1);
        }

        int of = sum_overflows(res, n);
        if (of == 1) {
            fprintf(stderr, "Long int overflow\n");
            exit(1);
        }
        if (of == -1) {
            fprintf(stderr, "Long int underflow\n");
            exit(1);
        }

        res += n;
    }
    return res;
}

void print_mean(long sum, long cnt) {
    double mean = sum / (cnt - 2.);
    const double EPSILON = 1E-5;
    double mean_intpart = (double) ((long) mean);
    if (fabs(mean - mean_intpart) < EPSILON)
        printf("= %ld\n", (long) mean);
    else printf("= %.3f\n", mean);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("%s", USAGE);
        return 1;
    }
    long res = 0;
    char mode;
    switch(mode = get_mode(argc, argv)) {
        case 'm':
            res = add_args(argc, argv, 2);
            print_mean(res, argc);
            break;

        case 'S':
        case 's':
            res = mode == 'S' ? add_args(argc, argv, 1) :
                                add_args(argc, argv, 2);
            printf("= %ld\n", res);
            break;

        case 'h':
            printf("%s", HELP);
            break;

        default:
            printf("%s", USAGE);
            return 1;
    }
    return 0;
}

