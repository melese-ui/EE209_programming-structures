/************************************
 Name: Melese Medhin
 Student Id: 20210727
 Assignment 2
 file: str.c
 ***********************************/

#include <assert.h>  /* for assert() */
#include <stdio.h>
#include "str.h"
#include <string.h>
#include <stdlib.h>

/* Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*-------------------------------------------------------------------*/
size_t StrGetLength(const char* pcSrc)
{
    assert(pcSrc);  /* NULL address, 0, and FALSE are identical. */

    const char *endPtr = pcSrc;
    while (*endPtr != '\0') {
        endPtr++;
    }

    return (size_t)(endPtr - pcSrc);
}

/*-------------------------------------------------------------------*/
/*
The StrCopy function first does argument validation using assort. 
It aborts te program if the arguments are NULL. Then the while loop
runs through the source string (pcSrc) until a null('\0') symbol ismet.
Meanwhile the characters are being copied to a destination 
array(pcDest). Finally, a null('\0') symbol is added to the pcDest
to indicate its end. It then returns a pointer to the 
pcDest.
*/
char *StrCopy(char *pcDest, const char* pcSrc)
{
    assert(pcDest);
    assert(pcSrc);

    char *destPtr = pcDest;
    const char *srcPtr = pcSrc;

    while (*srcPtr != '\0') {
        *destPtr = *srcPtr;
        srcPtr++;
        destPtr++;
    }

    *destPtr = '\0';

    return pcDest;
    //return strcpy(pcDest, pcSrc);
}

/*-------------------------------------------------------------------*/
/*
The StringCompare function also first does argument validation using
assort. It aborts te program if the arguments are NULL. Then the while
loop runs through the source string until a null('\0') symbol is met.
It then runs a while loop as long as the characters of the two strings
are the same, except when the specific character are '\0' where it 
returns zero because that is the end of the strings. In other cases,
it just increaments the pointer. When the corresponing characters 
are different, it returns the difference between their values.
*/
int StrCompare(const char* pcS1, const char* pcS2)
{
    assert(pcS1);
    assert(pcS2);

    while (*pcS1 == *pcS2) {
        if (*pcS1 == '\0') {
            return 0;
        }
        pcS1++;
        pcS2++;
    }

    return ((unsigned char)*pcS1 - (unsigned char)*pcS2);

    //return strcmp(pcS1, pcS2);
}

/*-------------------------------------------------------------------*/
/*The StrSearch function takes two arguments (pcHaystack 
  and pcNeedle). It first asserts both arguments   
  to check  if they arenot null. If it succesfully   
  finds the pcNeedle substring in pcHaystack, it will return
  a casted pointer that points at the  beginning of the pcNeedle 
  substring in the pcHaystack. Otherwise it returns NULL     

*/
char* StrSearch(const char *pcHaystack, const char *pcNeedle)
{
    assert(pcHaystack);
    assert(pcNeedle);

    while (*pcHaystack) {
        const char *start = pcHaystack;
        const char *pattern = pcNeedle;

        while (*pcHaystack && *pattern && *pcHaystack == *pattern) {
            pcHaystack++;
            pattern++;
        }

        if (*pattern == '\0') {
            return (char*)start;
        }

        pcHaystack = start + 1;
        pcNeedle = pattern;
    }

    return NULL;

    //return strstr(pcHaystack, pcNeedle);
}

/*-------------------------------------------------------------------*/
/*
The StringCompare function also first does argument validation using
assort. It aborts the program if the arguments are NULL. It then assigns
a pointer to the null('\0') at the end. It then copies the characters
of the source string(pcSrc) to the destination string(pcDest) until
the '\0' of the pcDest is reached.

*/
char *StrConcat(char *pcDest, const char *pcSrc)
{
    assert(pcDest);
    assert(pcSrc);

    char *destEnd = pcDest + StrGetLength(pcDest);

    while (*pcSrc != '\0') {
        *destEnd = *pcSrc;
        destEnd++;
        pcSrc++;
    }

    *destEnd = '\0';

    return pcDest;
    //return strcat(pcDest, pcSrc);
}
