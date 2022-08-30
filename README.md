# cpgtou
### Code page to Unicode one-way conversion

    int32_t cpgtou(cpg_t cpg, uint8_t cpt, uint8_t *xtra, const int32_t **mult)

### USAGE

The function generally returns the Unicode code point corresponding most
closely to the code point cpt in code page cpg.  However, since not all
code points in all code pages map 1:1 to Unicode, we also allow for
carry-over of high bytes (for double-byte encodings, e.g., Big5, GB2312,
or ShiftJIS) and multiple return.

### RETURN VALUE

- On successful 1:1 lookup, and on successful 2:1 lookup for multibyte
  encodings, the function returns the corresponding Unicode code point.

- If the specified code point does not exist in the given code page,
  or if it cannot be translated into Unicode, then the function returns
  cpNONE (defined as -1).

- If the specified code page is not yet supported, the function
  will return cpUNSP (defined as -2).

- If the (low) byte stored in cpt is the first half of a double-byte
  sequence (e.g., in ShiftJIS-based code pages), the function will
  return cpDBSQ (defined as -3).  The byte stored in cpt will be stored
  in the memory location pointed-to by xtra for use in the subsequent
  call.

- If the code point in cpt (whether single- or double-byte) codes for
  a character or sequence that must use multiple Unicode code points
  for translation, the function will return cpMULT (defined as -4)
  and the pointer whose address was passed in **mult will be set to
  the beginning of a 0L-terminated array of int32_t Unicode code
  points to cycle through.
  
### BITSC LICENSE NOTICE
                                            
CPGTOU – Code Page to Unicode   
Copyright (c) 2022 Joshua Lee Ockert <<torstenvl@gmail.com>>

THIS WORK IS PROVIDED "AS IS" WITH NO WARRANTY OF ANY KIND. THE IMPLIED
WARRANTIES OF MERCHANTABILITY, FITNESS, NON-INFRINGEMENT, AND TITLE ARE
EXPRESSLY DISCLAIMED. NO AUTHOR SHALL BE LIABLE UNDER ANY THEORY OF LAW
FOR ANY DAMAGES OF ANY KIND RESULTING FROM THE USE OF THIS WORK.

Permission to use, copy, modify, and/or distribute this work for any
purpose is hereby granted, provided this notice appears in all copies.
