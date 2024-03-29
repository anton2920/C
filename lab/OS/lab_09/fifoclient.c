/*****************************************************************************
 Excerpt from "Linux Programmer's Guide - Chapter 6"
 (C)opyright 1994-1995, Scott Burkett
 *****************************************************************************
 MODULE: fifoclient.c
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define FIFO_FILE "MYFIFO"

int main(int argc, char *argv[]) {

    /* Initializing variables */
    FILE *fp;

    if (argc != 2) {
        printf("USAGE: fifoclient [string]\n");
        exit(1);
    }

    if ((fp = fopen(FIFO_FILE, "w")) == NULL) {
        perror("fopen");
        exit(1);
    }

    fputs(argv[1], fp);

    fclose(fp);
} 