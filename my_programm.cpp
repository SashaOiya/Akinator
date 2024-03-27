#include "akin.h"

int my_getline_file ( char *buffer, FILE *fp )
{
    assert ( buffer != nullptr );
    assert ( fp != nullptr );

    int temp = 0;;
    size_t i = 0;

    while ( ( temp = fgetc ( fp ) ) != EOF && temp != '\n' ) {
        buffer[i++] = (char)temp;
    }

    if ( i > 0 || temp == '\n' ) {
        buffer[i] = '\0';

        return i;
    }

    return ERR_RLINE;
}

int my_getline_console ( char *buffer )
{
    assert ( buffer != nullptr );

    int temp = 0;
    size_t i = 0;

    while ( ( temp = getchar ( ) ) != '\n' && temp != EOF ) {
        buffer[i++] = (char)temp;
    }
    if ( i > 0 || temp == '\n' ) {
        buffer[i] = '\0';

        return i;
    }

    return ERR_RLINE;   // wtf
}

void print_color ( const char *line, enum Color_t COLOR, ... )
{
    HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );

    va_list args;
    va_start ( args, COLOR );

    SetConsoleTextAttribute ( hConsole, COLOR );

    vprintf ( line, args );

    SetConsoleTextAttribute ( hConsole, COLOR_WHITE );

    va_end ( args );
}

void reverse_array ( int arr[], int size ) {
    int start = 0;
    int end = size - 1;

    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;

        start++;
        end--;
    }
}
