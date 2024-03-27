#ifndef MY_PROG
#define MY_PROG

#ifdef DEBUGG
#define $ printf ( "function <%s> line <%d>\n ", __PRETTY_FUNCTION__, __LINE__ );
#else
#define $
#endif

enum Color_t {
    COLOR_GREEN  = 10,
    COLOR_WHITE  = 15,
    COLOR_RED    =  4,
    COLOR_PINK   = 12,
    COLOR_BLUE   =  9,
    COLOR_YELLOW = 14
};

enum Errors_t {
    NO_ERR    = 0,
    ERR_FREAD = 1,
    ERR_INPUT = 2,
    ERR_FOPEN = 3,
    ERR_RLINE = 4,
    ERR_CALLO = 5,
    ERR_CYCLE = 6
};

int  my_getline_file    ( char *buffer, FILE *f  );
int  my_getline_console ( char *buffer );
void print_color       ( const char *line, enum Color_t COLOR, ... );
void reverse_array ( int arr[], int size );

#endif    // MY_PROG
