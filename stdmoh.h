#ifndef   STDMOH_H_INCLUDED
#define   STDMOH_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>



typedef unsigned char bool_t;
#define TRUE          1U
#define FALSE         0U
#define true          1U
#define false         0U

typedef char          char_t;
typedef int8_t        char8_t;
typedef int16_t       char16_t;
typedef int32_t       char32_t;
typedef int64_t       char64_t;

typedef unsigned char uchar_t;
typedef uint8_t       uchar8_t;
typedef uint16_t      uchar16_t;
typedef uint32_t      uchar32_t;
typedef uint64_t      uchar64_t;

typedef char*         str_t;
typedef int8_t*       str8_t;
typedef int16_t*      str16_t;
typedef int32_t*      str32_t;
typedef int64_t*      str64_t;

typedef uchar_t*      ustr_t;
typedef uint8_t*      ustr8_t;
typedef uint16_t*     ustr16_t;
typedef uint32_t*     ustr32_t;
typedef uint64_t*     ustr64_t;

typedef char*         string_t;
typedef int8_t*       string8_t;
typedef int16_t*      string16_t;
typedef int32_t*      string32_t;
typedef int64_t*      string64_t;

typedef uchar_t*      ustring_t;
typedef uint8_t*      ustring8_t;
typedef uint16_t*     ustring16_t;
typedef uint32_t*     ustring32_t;
typedef uint64_t*     ustring64_t;

typedef float         real32_t;
typedef double        real64_t;
typedef long double   real80_t;



#endif // STDMOH_H_INCLUDED
