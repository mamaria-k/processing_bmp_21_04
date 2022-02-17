#ifndef HW_1_STEGO_H
#define HW_1_STEGO_H

#include "bmp.h"


void coding_message(bmp_t* p_bmp, const char* message_txt, const char* key_txt);
void decoding_message(bmp_t* p_bmp, const char* message_txt, const char* key_txt);


#endif //HW_1_STEGO_H
