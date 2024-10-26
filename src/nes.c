#include <getopt.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int opt_disassemble = 0, opt_run = 0;

    int c;
    while ((c = getopt(argc, argv, "dvrites:a:")) != -1) {
        switch (c) {
            case 'd':
                opt_disassemble = 1;
                break;
            case 'r':
                opt_run = 1;
                break;
            default:
                fprintf(stderr, "ERROR: Invalid flag %c\n", c);
                return -1;
        }
    }

    if (opt_disassemble == 1) {
        printf("Disassemble\n");
    } else if (opt_run == 1) {
        printf("Run\n");
    } else {
        printf("No valid arguments provided\n");
    }
    printf("Done!\n");
}
