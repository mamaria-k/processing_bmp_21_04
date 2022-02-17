#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"


static int32_t addition_row(int32_t width) {
    int32_t r = 3 * width % 4;
    if (r == 0) return 0;
    return 4 - r;
}


static void init_bmp(bmp_t* p_bmp, bmp_t* p_new_bmp, int32_t new_w, int32_t new_h) {
    p_new_bmp->file_header = p_bmp->file_header;
    p_new_bmp->info = p_bmp->info;
    p_new_bmp->info.biWidth = new_w;
    p_new_bmp->info.biHeight = new_h;
    p_new_bmp->info.biSizeImage = new_h * (new_w * 3 + addition_row(new_w));
    p_new_bmp->file_header.bfOffBits = sizeof(bmp_file_header_t) + sizeof(bmp_info_header_t);
    p_new_bmp->file_header.bfSize = p_new_bmp->file_header.bfOffBits + p_new_bmp->info.biSizeImage;

    p_new_bmp->image = (pixel_t**) malloc(p_new_bmp->info.biHeight * sizeof(pixel_t*));
    pixel_t* image= (pixel_t*) malloc(p_new_bmp->info.biHeight * p_new_bmp->info.biWidth * sizeof(pixel_t));
    if (p_new_bmp->image == NULL || image == NULL) {
        printf("%s\n", "Error! Memory not allocated");
        exit(1);
    }
    for (int i = p_new_bmp->info.biHeight - 1; i >= 0; i--) {
        p_new_bmp->image[i] = image;
        image += p_new_bmp->info.biWidth;
    }
}


void free_bmp(bmp_t* p_bmp) {
    free(p_bmp->image[p_bmp->info.biHeight - 1]);
    free(p_bmp->image);
}


void load_bmp(const char* in_filename, bmp_t* p_bmp) {
    FILE* in_file_bmp = fopen(in_filename, "rb");
    if (in_file_bmp == NULL) {
        printf("%s\n", "Error opening file!");
        exit(1);
    }

    fread(&p_bmp->file_header, sizeof(bmp_file_header_t), 1, in_file_bmp);
    fread(&p_bmp->info, sizeof(bmp_info_header_t), 1, in_file_bmp);

    fseek(in_file_bmp, p_bmp->file_header.bfOffBits, SEEK_SET);
    p_bmp->image = (pixel_t**) malloc(p_bmp->info.biHeight * sizeof(pixel_t*));
    pixel_t* image= (pixel_t*) malloc(p_bmp->info.biHeight * p_bmp->info.biWidth * sizeof(pixel_t));
    if (p_bmp->image == NULL || image == NULL) {
        fclose(in_file_bmp);
        printf("%s\n", "Error! Memory not allocated");
        exit(1);
    }
    for (int i = p_bmp->info.biHeight - 1; i >= 0; i--) {
        p_bmp->image[i] = image;
        fread(p_bmp->image[i], sizeof(pixel_t), p_bmp->info.biWidth, in_file_bmp);

        image += p_bmp->info.biWidth;
        fseek(in_file_bmp, addition_row(p_bmp->info.biWidth), SEEK_CUR);
    }


    fclose(in_file_bmp);

}


void crop(bmp_t* p_bmp, bmp_t* p_cropped_bmp, int x, int y, int w, int h) {
    init_bmp(p_bmp, p_cropped_bmp, w, h);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            p_cropped_bmp->image[i][j] = p_bmp->image[y + i][x + j];
        }
    }

    free_bmp(p_bmp);
}


void rotate(bmp_t* p_bmp, bmp_t* p_rotated_bmp) {
    init_bmp(p_bmp, p_rotated_bmp, p_bmp->info.biHeight, p_bmp->info.biWidth);
    for (int i = 0; i < p_rotated_bmp->info.biHeight; i++) {
        for (int j = 0; j < p_rotated_bmp->info.biWidth; j++) {
            p_rotated_bmp->image[i][j] = p_bmp->image[p_bmp->info.biHeight - 1 - j][i];
        }
    }

    free_bmp(p_bmp);
}


void save_bmp(const char* out_filename, bmp_t* p_bmp) {
    FILE* out_file_bmp = fopen(out_filename, "wb");
    if (out_file_bmp == NULL) {
        printf("%s\n", "Error saving file!");
        exit(1);
    }

    fwrite(&p_bmp->file_header, sizeof(bmp_file_header_t), 1, out_file_bmp);
    fwrite(&p_bmp->info, sizeof(bmp_info_header_t), 1, out_file_bmp);

    fseek(out_file_bmp, p_bmp->file_header.bfOffBits, SEEK_SET);
    char addition_of_zeros[4] = {0};
    for (int i = p_bmp->info.biHeight - 1; i >= 0 ; i--) {
        fwrite(p_bmp->image[i], sizeof(pixel_t), p_bmp->info.biWidth, out_file_bmp);
        fwrite(addition_of_zeros, 1, addition_row(p_bmp->info.biWidth), out_file_bmp);
    }

    free_bmp(p_bmp);
    fclose(out_file_bmp);
}

