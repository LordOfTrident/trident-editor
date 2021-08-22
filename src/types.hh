#include <cstdint>
#include <string>
#include <vector>

using std::vector;

typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef uint32_t ui32;
typedef uint16_t ui16;
typedef uint8_t  ui8;

typedef size_t   word;
typedef intmax_t sword;

typedef std::string  str;
typedef std::wstring wstr;

typedef char    ch;
typedef wchar_t wch;
#ifdef NCURSES_WIDECHAR
typedef cchar_t cch;
#endif

typedef ui8  color;
typedef ui16 flags;

typedef vector <ui8> colors;
