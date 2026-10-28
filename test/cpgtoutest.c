#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "cpgtou.h"

static inline unsigned char *utf8_from_cdpt(int32_t c) {
    _Thread_local static unsigned char u[5];
    if (c<0 || (0xD800 <= c&&c <= 0xDBFF)) u[0]= '\0';
    else if (c < 0x80) {               // Up to 7 bits
        u[0]= (unsigned char)(c>>0   & 0x7F);  // 7 bits –> 0xxxxxxx
        u[1]= '\0';
    }
    else if (c < 0x800) {              // Up to 11 bits
        u[0]= (unsigned char)((c>>6  & 0x1F)|0xC0);  // 5 bits –> 110xxxxx
        u[1]= (unsigned char)((c     & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[2]= '\0';
    }
    else if (c < 0x10000) {            // Up to 16 bits
        u[0]= (unsigned char)((c>>12 & 0x0F)|0xE0);  // 4 bits –> 1110xxxx
        u[1]= (unsigned char)((c>>6  & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[2]= (unsigned char)((c     & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[3]= '\0';
    }
    else if (c < 0x110000) {           // Up to 21 bits
        u[0]= (unsigned char)((c>>18 & 0x07)|0xF0);  // 3 bits –> 11110xxx
        u[1]= (unsigned char)((c>>12 & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[2]= (unsigned char)((c>>6  & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[3]= (unsigned char)((c     & 0x3F)|0x80);  // 6 bits –> 10xxxxxx
        u[4]= '\0';
    }
    else u[0]= '\0';
    return u;
}

static unsigned char input[] = { 0x94, 0x45, 0x8e, 0xd2, 0x90, 0xed, 0x8e, 
                                 0x6d, 0x82, 0xaa, 0x8e, 0x98, 0x82, 0xf0, 
                                 0x93, 0x7c, 0x82, 0xb7, 0x81, 0x42, 0x00 };
static unsigned char output[256] = { 0 };

#define ENCOD(x) (utf8_from_cdpt(x))
#define CHECK(x,y) (assert(!strcmp(x,y)))
#define STRINGADD(s1, s2, max) (strncat((char *)s1, (char *)s2, (max - strlen((char *)s1) - 1)))

int main(void) {
    int32_t uccp;
    const int32_t *mult = 0;
    uint8_t xtra = 0U;

    size_t i;

    unsigned char *u;

    for (i = 0; i < strlen((char *)input); i++) {
        uccp = cpgtou(cpgfromcharsetnum(128), input[i], &xtra, &mult);
        if (uccp >= 0) { u = ENCOD(uccp); STRINGADD(output, u, 256); }
        if (uccp == cpMULT) while(*mult) { u = ENCOD(*mult); STRINGADD(output, u, 256); }
    }

    CHECK((char *)output, u8"忍者戦士が侍を倒す。");

    printf("\n");
    printf("Converted string:   \'%s\'\n", (char *)output);
    printf("Comparison string:  \'%s\'\n", u8"忍者戦士が侍を倒す。");

    return 0;
}
