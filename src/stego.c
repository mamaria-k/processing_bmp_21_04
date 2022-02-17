#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stego.h"
#include "bmp.h"


static void char_to_bin(int n, char* bin) {
    int count = 0;
    while (count < 5) {
        if (n % 2 == 0) bin[count] = '0';
        else bin[count] = '1';
        n /= 2;
        count++;
    }
}


static void change_pixel(u_char* color_channel_of_pixel, char bit) {
    if (bit == '0') *color_channel_of_pixel &= ~1;
    else *color_channel_of_pixel |= 1;
}


static int coding_bit(bmp_t* p_bmp, int x, int y, char color, char bit) {
    if (x < 0 || y < 0 || x >= p_bmp->info.biWidth || y >= p_bmp->info.biHeight) return 1;
    if (color == 'R') change_pixel(&p_bmp->image[y][x].red, bit);
    else if (color == 'G') change_pixel(&p_bmp->image[y][x].green, bit);
    else if (color == 'B') change_pixel(&p_bmp->image[y][x].blue, bit);
    else return 1;
    return 0;
}


static int coding_char(bmp_t* p_bmp, FILE* key, char* char_as_bin) {
    int x, y;
    char color;
    int count = 0;
    while (count < 5) {
        if (fscanf(key, "%d %d %c", &x, &y, &color) == 3) {
            int incorrect_key = coding_bit(p_bmp, x, y, color, char_as_bin[count]);
            if (incorrect_key) return 1;
            count++;
        }
        else return 1;
    }
    return 0;
}


void coding_message(bmp_t* p_bmp, const char* message_txt, const char* key_txt) {
    FILE* msg = fopen(message_txt, "r");
    FILE* key = fopen(key_txt, "r");
    if (msg == NULL || key == NULL) {
        printf("%s\n", "Error opening file");
        exit(1);
    }

    int c;
    while ((c = fgetc(msg)) != EOF) {
        char char_as_bin[5];

        if (c > 64 && c < 91) char_to_bin(c - 65, char_as_bin);   //capital letters
        else if (c == 32) char_to_bin(26, char_as_bin);           //space
        else if (c == 46) char_to_bin(27, char_as_bin);           //dot
        else if (c == 44) char_to_bin(28, char_as_bin);           //comma
        else {
            printf("%s\n", "Incorrect message format!");
            fclose(msg);
            fclose(key);
            exit(1);
        }

        int incorrect_key = coding_char(p_bmp, key, char_as_bin);
        if (incorrect_key) {
            printf("%s\n", "Incorrect key!");
            fclose(msg);
            fclose(key);
            exit(1);
        }
    }

    fclose(msg);
    fclose(key);
}


static int bin_to_char(const char* bin) {
    int n = 0;
    int count = 4;
    while (count >= 0) {
        n *= 2;
        if (bin[count] == '1') n += 1;
        count--;
    }
    return n;
}


static char analysis_pixel(u_char color_channel_of_pixel) {
    if (color_channel_of_pixel & 1) {
        return '1';
    }
    else return '0';
}


static char extract_bit(bmp_t* p_bmp, int x, int y, char color) {
    if (x < 0 || y < 0 || x >= p_bmp->info.biWidth || y >= p_bmp->info.biHeight) return 1;
    if (color == 'R') return analysis_pixel(p_bmp->image[y][x].red);
    else if (color == 'G') return analysis_pixel(p_bmp->image[y][x].green);
    else if (color == 'B') return analysis_pixel(p_bmp->image[y][x].blue);
    else return 1;
}


void decoding_message(bmp_t* p_bmp, const char* message_txt, const char* key_txt) {
    FILE* msg = fopen(message_txt, "w");
    FILE* key = fopen(key_txt, "r");
    if (msg == NULL || key == NULL) {
        printf("%s\n", "Error opening file");
        exit(1);
    }

    int x, y;
    char color;
    int count = 0;
    char char_as_bin[5];
    while (fscanf(key, "%d %d %c", &x, &y, &color) == 3) {
        char bit = extract_bit(p_bmp, x, y, color);
        if (bit == 1) {
            printf("%s\n", "Incorrect key!");
            fclose(msg);
            fclose(key);
            exit(1);
        }
        char_as_bin[count] = bit;
        count++;

        if (count == 5) {
            count = 0;
            int c = bin_to_char(char_as_bin);
            if (c < 26) fputc(c + 65, msg);
            else if (c == 26) fputc(32, msg);
            else if (c == 27) fputc(46, msg);
            else if (c == 28) fputc(44, msg);
            else {
                printf("%s\n", "Decode error!");
                fclose(msg);
                fclose(key);
                exit(1);
            }
        }
    }

    fclose(msg);
    fclose(key);
}




