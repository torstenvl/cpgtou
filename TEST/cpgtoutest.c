#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../cpgtou.h"

static void encode_utf8(int32_t c, char utf8[5]) {
    char *u = utf8;
    if (c < 0) {
        u[0]= '\0';
    } else 
    if (c < 0b000000000000010000000) { // Up to 7 bits
        u[0]= (char)(c>>0  & 0b01111111 | 0b00000000);  // 7 bits –> 0xxxxxxx
        u[1]= '\0';
    } 
    else if (c < 0b000000000100000000000) { // Up to 11 bits
        u[0]= (char)(c>>6  & 0b00011111 | 0b11000000);  // 5 bits –> 110xxxxx
        u[1]= (char)(c>>0  & 0b00111111 | 0b10000000);  // 6 bits –> 10xxxxxx
        u[2]= '\0';
    } 
    else if (c < 0b000010000000000000000) { // Up to 16 bits
        u[0]= (char)(c>>12 & 0b00001111 | 0b11100000);  // 4 bits –> 1110xxxx
        u[1]= (char)(c>>6  & 0b00111111 | 0b10000000);  // 6 bits –> 10xxxxxx
        u[2]= (char)(c>>0  & 0b00111111 | 0b10000000);  // 6 bits –> 10xxxxxx
        u[3]= '\0';
    } 
    else if (c < 0b100010000000000000000) { // Up to 21 bits
        u[0]= (char)(c>>18 & 0b00000111 | 0b11110000);  // 3 bits –> 11110xxx
        u[1]= (char)(c>>12 & 0b00111111 | 0b10000000);  // 6 bits –> 10xxxxxx
        u[2]= (char)(c>>6  & 0b00111111 | 0b10000000);  // 6 bits –> 10xxxxxx
        u[3]= (char)(c>>0  & 0b00111111 | 0b10000000);  // 6 bits –> 10xxxxxx
        u[4]= '\0';
    } 
    else {
        u[0]= '\0';
    }
}

static unsigned char input[] = { 0x94, 0x45, 0x8e, 0xd2, 0x90, 0xed, 0x8e, 
                                 0x6d, 0x82, 0xaa, 0x8e, 0x98, 0x82, 0xf0, 
                                 0x93, 0x7c, 0x82, 0xb7, 0x81, 0x42, 0x00 };
static unsigned char output[256] = { 0 };

#define ENCOD(x,y) (encode_utf8(y,x))
#define CHECK(x,y) (assert(!strcmp(x,y)))
#define STRINGADD(s1, s2, max) (strncat((char *)s1, (char *)s2, (max - strlen((char *)s1) - 1)))

int main(void) {
    int32_t uccp;
    const int32_t *mult;
    uint8_t xtra = 0U;

    size_t i;

    char u[5];

    for (i = 0; i < strlen((char *)input); i++) {
        uccp = cpgtou(cpgfromcharsetnum(128), input[i], &xtra, &mult);
        if (uccp >= 0) { ENCOD(u, uccp); STRINGADD(output, u, 256); }
        if (uccp == cpMULT) while(*mult) { ENCOD(u, *mult); STRINGADD(output, u, 256); }
    }

    CHECK((char *)output, u8"忍者戦士が侍を倒す。");

    return 0;
}
