#include <stdio.h>
#include "../cpgtou.h"

void encode_utf8(int32_t c, char utf8[5]) {
    if (c < 0x80) {
	    // (__ & 01111111)|00000000  ==> 0xxx xxxx
	    utf8[0] = (char)(c>>0  &  0x7F) | (char)0x00;  
		utf8[1] = '\0';
    }
    else if (c < 0x0800) {
        // (__ & 00011111)|11000000  ==> 110x xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        utf8[0] = (char)(c>>6 & 0x1F) | (char)0xC0;
        utf8[1] = (char)(c>>0 & 0x3F) | (char)0x80;
        utf8[2] = '\0';
    }
    else if (c < 0x010000) {
        // (__ & 00001111)|11100000  ==> 1110 xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        utf8[0] = (char)(c>>12 &  0x0F) | (char)0xE0; 
        utf8[1] = (char)(c>>6  &  0x3F) | (char)0x80;
        utf8[2] = (char)(c>>0  &  0x3F) | (char)0x80;
        utf8[3] = '\0';
    }
    else if (c < 0x110000) {
        // (__ & 00000111)|11110000  ==> 1111 0xxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        utf8[0] = (char)(c>>18 &  0x07) | (char)0xF0;
        utf8[1] = (char)(c>>12 &  0x3F) | (char)0x80;
        utf8[2] = (char)(c>>6  &  0x3F) | (char)0x80;
        utf8[3] = (char)(c>>0  &  0x3F) | (char)0x80;
        utf8[4] = '\0';
    }
}


int main(void) {
    int c;
    uint8_t xtra;
    const int32_t *mult;
    
    int32_t u;
    
    char utf8[5] = { 0 };
    
    while ((c = fgetc(stdin)) != EOF) {
        u = cpgtou(CPG_437, c, &xtra, &mult);
        switch (u) {
            case cpNONE:
            case cpUNSP:
            case cpDBSQ:
                break;
            case cpMULT:
                while (*mult != 0) {
                    encode_utf8(*mult, utf8);
                    printf("%s", utf8);
                    mult++;
                }
                break;
            default:
                encode_utf8(u, utf8);
                printf("%s", utf8);
                break;
        }
    }

    return 0;
}

