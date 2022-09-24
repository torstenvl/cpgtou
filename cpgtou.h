/*---------------------------------------------------------------------------*\
|                                                                             |
|                        CPGTOU – Code Page to Unicode                        |
|                                                                             |
|-----------------------------------------------------------------------------|
|                                                                             |
|   Copyright (c) 2022 Joshua Lee Ockert <torstenvl@gmail.com>                |
|                      https://github.com/torstenvl                           |
|                                                                             |
|   THIS WORK IS PROVIDED "AS IS" WITH NO WARRANTY OF ANY KIND. THE IMPLIED   |
|   WARRANTIES OF MERCHANTABILITY, FITNESS, NON-INFRINGEMENT, AND TITLE ARE   |
|   EXPRESSLY DISCLAIMED. NO AUTHOR SHALL BE LIABLE UNDER ANY THEORY OF LAW   |
|   FOR ANY DAMAGES OF ANY KIND RESULTING FROM THE USE OF THIS WORK.          |
|                                                                             |
|   Permission to use, copy, modify, and/or distribute this work for any      |
|   purpose is hereby granted, provided this notice appears in all copies.    |
|                                                                             |
\*---------------------------------------------------------------------------*/


#ifndef CPGTOU_H__
#define CPGTOU_H__

#include <stddef.h>
#include <inttypes.h>

#define cpNONE   -1      // No Unicode equivalent, possible invalid code point
#define cpUNSP   -2      // Unsupported code page
#define cpDBSQ   -3      // cpt is the first half of a double-byte sequence
#define cpMULT   -4      // Look to last argument for a 0L-terminated list

typedef enum cpg_t {
    CPG_42    = 42,       // ❌ Windows Symbol
    CPG_437   = 437,      // OEM/IBM United States
    CPG_708   = 708,      // ASMO Arabic (ASMO 708)
    CPG_709   = 709,      // Arabic (ASMO 449+, BCON V4) 
    CPG_709P  = 60709,    // Progress Software variant of Code Page 709
    CPG_710   = 710,      // ❌❌❌ WONTFIX Arabic (transparent Arabic)
    CPG_711   = 711,      // ❌❌❌ WONTFIX Arabic (Nafitha Enhanced)
    CPG_720   = 720,      // DOS Arabic (transparent ASMO)
    CPG_819   = 819,      // DOS Windows 3.1 (U.S. and W. Europe)
    CPG_850   = 850,      // OEM/IBM Multilingual
    CPG_852   = 852,      // OEM/IBM Eastern European
    CPG_860   = 860,      // OEM/IBM Portuguese
    CPG_862   = 862,      // DOS Hebrew
    CPG_863   = 863,      // OEM/IBM French Canadian
    CPG_864   = 864,      // DOS Arabic
    CPG_865   = 865,      // DOS Norwegian
    CPG_866   = 866,      // DOS Soviet Union
    CPG_874   = 874,      // DOS Thai
    CPG_932   = 932,      // DOS Japanese
    CPG_936   = 936,      // ❌ DOS Simpl. Chinese (GB2312)
    CPG_949   = 949,      // ❌ DOS Korean
    CPG_950   = 950,      // DOS Trad. Chinese (Big5)
    CPG_1250  = 1250,     // Windows E. European
    CPG_1251  = 1251,     // Windows Cyrillic
    CPG_1252  = 1252,     // Windows W. European
    CPG_1253  = 1253,     // Windows Greek
    CPG_1254  = 1254,     // Windows Turkish
    CPG_1255  = 1255,     // Windows Hebrew
    CPG_1256  = 1256,     // Windows Arabic
    CPG_1257  = 1257,     // Windows Baltic
    CPG_1258  = 1258,     // Windows Vietnamese
    CPG_1361  = 1361,     // ❌ Windows Korean (Johab)
    CPG_10000 = 10000,    // Mac Roman
    CPG_10001 = 10001,    // Mac Japan
    CPG_10002 = 10002,    // ❌ Mac Trad. Chinese (Big5)
    CPG_10003 = 10003,    // ❌ Mac Hangul (Korean)
    CPG_10004 = 10004,    // Mac Arabic
    CPG_10005 = 10005,    // Mac Hebrew
    CPG_10006 = 10006,    // Mac Greek
    CPG_10007 = 10007,    // Mac Cyrillic/Russian
    CPG_10008 = 10008,    // ❌ Mac Simpl. Chinese (GB2312)
    CPG_10021 = 10021,    // Mac Thai
    CPG_10029 = 10029,    // Mac Latin2 E. Europe
    CPG_10081 = 10081,    // Mac Turkish

    // The following code pages were added in Windows 2000 and correspond to 
    // the ISCII character sets. Reportedly, they cannot be translated to    
    // Unicode using a simple table-based approach but are more 'algorithmic'
    // https://archives.miloush.net/michkap/archive/2005/10/28/486232.html   

    // These code pages are not supported by macOS Catalina TextEdit, macOS
    // Pages 11.1, Microsoft Word for Mac 2019, Microsoft Word for Windows
    // 97, LibreOffice 6.5, LibreOffice 7.2, or LibreOffice 7.3. They ARE
    // supported, at least in part, by Windows 7 WordPad. That makes them
    // low priority at the moment.  Additionally, these code pages are
    // implementations of the ISCII standard, which includes significant
    // formatting and code-switching WITHIN each code page, e.g., there are
    // byte sequences in Code Page 57002 that mean that the following
    // byte sequences should be treated as if they are in Code Page 57003. 
    // This makes high-fidelity translation into Unicode more complicated.
    // TARGET: SOME FUTURE VERSION

    CPG_57002 = 57002,    // ❌❌ Devanagari (Hindi, Sanskrit, Marathi, Nepalese)
    CPG_57003 = 57003,    // ❌❌ Bengali
    CPG_57004 = 57004,    // ❌❌ Tamil
    CPG_57005 = 57005,    // ❌❌ Telugu
    CPG_57006 = 57006,    // ❌❌ Assamese
    CPG_57007 = 57007,    // ❌❌ Oriya
    CPG_57008 = 57008,    // ❌❌ Kannada
    CPG_57009 = 57009,    // ❌❌ Malayalam
    CPG_57010 = 57010,    // ❌❌ Gujarati
    CPG_57011 = 57011,    // ❌❌ Punjabi

    CHSET_0   = 1252,     // Windows W. European
    CHSET_1   = 1252,     // Windows W. European
    CHSET_2   = 42,       // Symbol
    CHSET_77  = 10000,    // Mac Roman
    CHSET_78  = 10001,    // Mac Japan (ShiftJIS)
    CHSET_79  = 10003,    // Mac Hangul 
    CHSET_80  = 10008,    // Mac GB2312
    CHSET_81  = 10002,    // Mac Big5
    CHSET_82  = cpUNSP,   // ❌❌❌ WONTFIX Old Mac Johab        
    CHSET_83  = 10005,    // Mac Hebrew
    CHSET_84  = 10004,    // Mac Arabic
    CHSET_85  = 10006,    // Mac Greek
    CHSET_86  = 10081,    // Mac Turkish
    CHSET_87  = 10021,    // Mac Thai
    CHSET_88  = 10029,    // Mac Latin2 E. Europe
    CHSET_89  = 10007,    // Mac Cyrillic/Russian
    CHSET_128 = 932,      // DOS ShiftJIS
    CHSET_129 = 949,      // DOS Hangul
    CHSET_130 = 1361,     // Windows Korean (Johab)
    CHSET_134 = 936,      // DOS Simpl. Chinese GB2312
    CHSET_136 = 950,      // DOS Trad. Chinese (Big5)
    CHSET_161 = 1253,     // Windows Greek
    CHSET_162 = 1254,     // Windows Turkish
    CHSET_163 = 1258,     // Windows Vietnamese
    CHSET_177 = 1255,     // Windows Hebrew
    CHSET_178 = 1256,     // Windows Arabic
    CHSET_179 = cpUNSP,   // ❌❌❌ WONTFIX Old Arabic Traditional
    CHSET_180 = cpUNSP,   // ❌❌❌ WONTFIX Old Arabic User
    CHSET_181 = cpUNSP,   // ❌❌❌ WONTFIX Old Hebrew User
    CHSET_186 = 1257,     // Windows Baltic
    CHSET_204 = 1251,     // Windows Cyrillic
    CHSET_222 = 874,      // DOS Thai
    CHSET_238 = 1250,     // Windows E. European
    CHSET_254 = 437,      // IBM United States
    CHSET_255 = 850,      // IBM Multilingual


    ANSI      = 1252,     // Western European
    MAC       = 10000,    // Mac Roman
    PC        = 437,      // IBM United States
    PCA       = 850,      // IBM Multilingual
    MACROMAN  = 10000,    // Mac Roman
    NONE      = cpNONE,
    UNSUPPORTED = cpUNSP
} cpg_t;



cpg_t cpgfromcpgnum(int32_t n);
cpg_t cpgfromcharsetnum(int32_t n);
int32_t cpgtou(cpg_t cpg, uint8_t cpt, uint8_t *xtra, const int32_t **mult);



#endif
