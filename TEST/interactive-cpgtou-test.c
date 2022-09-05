#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include "../../STATIC/cpgtou.h"

static void encode_utf8(int32_t c, char utf8[5]) {
    if (c < 0x80) {
        // (__ & 01111111)|00000000  ==> 0xxx xxxx
        utf8[0] = (char)(c>>0  &  0x7F) | 0x00;  
        utf8[1] = '\0';
    }
    else if (c < 0x0800) {
        // (__ & 00011111)|11000000  ==> 110x xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        utf8[0] = (char)(c>>6 & 0x1F) | 0xC0;
        utf8[1] = (char)(c>>0 & 0x3F) | 0x80;
        utf8[2] = '\0';
    }
    else if (c < 0x010000) {
        // (__ & 00001111)|11100000  ==> 1110 xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        utf8[0] = (char)(c>>12 &  0x0F) | 0xE0; 
        utf8[1] = (char)(c>>6  &  0x3F) | 0x80;
        utf8[2] = (char)(c>>0  &  0x3F) | 0x80;
        utf8[3] = '\0';
    }
    else if (c < 0x110000) {
        // (__ & 00000111)|11110000  ==> 1111 0xxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        // (__ & 00111111)|10000000  ==> 10xx xxxx
        utf8[0] = (char)(c>>18 &  0x07) | 0xF0;
        utf8[1] = (char)(c>>12 &  0x3F) | 0x80;
        utf8[2] = (char)(c>>6  &  0x3F) | 0x80;
        utf8[3] = (char)(c>>0  &  0x3F) | 0x80;
        utf8[4] = '\0';
    }
}

static inline char *inputline(char *buf, size_t n, FILE *fp) {
    if (fgets(buf, n, fp) == NULL) return NULL;
    if (strlen(buf) < 1) return buf;     // Go ahead and return empty string
    if (buf[strlen(buf)-1] == '\n') {    // If we got the newline, NUL it
        buf[strlen(buf)-1] = '\0';
    } else {                             // If we didn't get the newline
        scanf("%*[^\n]");                //   Read everything UP TO newline
        scanf("%*c");                    //   And then one more byte
    }

    return buf;
}

void buffertohexbytes(char *buffer, int *hi, int *lo) {
    char *p;
    size_t goodchars;
    char hex[] = "0123456789abcdef0123456789ABCDEF";
    
    *hi = -1;
    *lo = -1;
    
    for (p = buffer; p < buffer+strlen(buffer); p += strcspn(p, hex)) {
        goodchars = strspn(p, hex);
        if (goodchars >= 4) {
            *hi = ((strchr(hex, p[0]) - hex) % 16) << 4;
            *hi = *hi + ((strchr(hex, p[1]) - hex) % 16);
            *lo = ((strchr(hex, p[2]) - hex) % 16) << 4;
            *lo = *lo + ((strchr(hex, p[3]) - hex) % 16);
            break;
        } else {
            p += goodchars;
        }
    }

    for (p = buffer; p < buffer+strlen(buffer); p += strcspn(p, hex)) {
        goodchars = strspn(p, hex);
        if (goodchars >= 2) {
            *hi = ((strchr(hex, p[0]) - hex) % 16) << 4;
            *hi = *hi + ((strchr(hex, p[1]) - hex) % 16);
            break;
        } else {
            p += goodchars;
        }
    }
}

int main(void) {
    char buffer[64];
    char utf8[5];
    int cpg;
    int32_t codepoint;
    int32_t extra = -1;
    int hi;
    int lo;

    printf("Enter a codepage:  ");
    scanf(" %d", &cpg);
    scanf("%*[^\n]");
    scanf("%*c");


    printf("Enter a single- or double-byte sequence:  ");
    inputline(buffer, 64, stdin);
    buffertohexbytes(buffer, &hi, &lo);


    codepoint = cpgtou(hi, cpg, &extra);
    if (codepoint == cpCAGN) codepoint = cpgtou(lo, cpg, &extra);


    if (codepoint == cpNONE) {
        printf("No Unicode mapping for that code point.\n");
    } else if (codepoint == cpSQER) {
        printf("No Unicode mapping due to invalid encoding in Code Page %d.\n", cpg);
    } else if (codepoint >= 0) {
        encode_utf8(codepoint, utf8);
        printf("Equal to Unicode code point U+%04" PRIX32" (%s).\n", codepoint, utf8);
    } else {
        printf("Unknown error getting Unicode mapping\n");
    }

    return 0;
}