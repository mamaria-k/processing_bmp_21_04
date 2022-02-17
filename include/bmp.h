#ifndef HW_1_BMP_H
#define HW_1_BMP_H

#include <stdint.h>


#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} bmp_file_header_t;


typedef struct tagBITMAPINFOHEADER {
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t  biPlanes;
    uint16_t  biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} bmp_info_header_t;


typedef struct pixel {
    u_char blue;
    u_char green;
    u_char red;
} pixel_t;


typedef struct bmp {
    bmp_file_header_t file_header;
    bmp_info_header_t info;
    pixel_t** image;
} bmp_t;

#pragma pack(pop)


void load_bmp(const char* in_filename, bmp_t* p_bmp);
void crop(bmp_t* p_bmp, bmp_t* p_cropped_bmp, int x, int y, int w, int h);
void rotate(bmp_t* p_bmp, bmp_t* p_rotated_bmp);
void save_bmp(const char* out_filename, bmp_t* p_bmp);
void free_bmp(bmp_t* p_bmp);


#endif //HW_1_BMP_H
