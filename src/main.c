#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bmp.h"
#include "stego.h"


int main(int argc, char** argv) {
    bmp_t bmp;
    load_bmp(argv[2], &bmp);

    if (strcmp(argv[1], "crop-rotate") == 0) {
        if (argc != 8) {
            printf("%s\n", "Incorrect number of arguments");
            return 1;
        }
        int x = atoi(argv[4]);
        int y = atoi(argv[5]);
        int w = atoi(argv[6]);
        int h = atoi(argv[7]);
        if (x < 0 || y < 0 || w < 1 || h < 1 || (x + w) > bmp.info.biWidth || (y + h) > bmp.info.biHeight) return 1;

        bmp_t cropped_bmp, rotated_bmp;
        crop(&bmp, &cropped_bmp, x, y, w, h);
        rotate(&cropped_bmp, &rotated_bmp);
        save_bmp(argv[3], &rotated_bmp);
    }

    else if (strcmp(argv[1], "insert") == 0) {
        if (argc != 6) {
            printf("%s\n", "Incorrect number of arguments");
            return 1;
        }

        coding_message(&bmp, argv[5], argv[4]);
        save_bmp(argv[3], &bmp);
    }

    else if (strcmp(argv[1], "extract") == 0) {
        if (argc != 5) {
            printf("%s\n", "Incorrect number of arguments");
            return 1;
        }

        decoding_message(&bmp, argv[4], argv[3]);
        free_bmp(&bmp);
    }
    else return 1;
    return 0;
}

