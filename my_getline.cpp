#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int my_getline ( char *buffer, FILE *f );
int getline_file ( char *buffer, FILE *fp );
int my_getline_console ( char *buffer );

int main ()
{
    FILE *f = fopen ( "test.txt", "r" );
    char buffer[50] = {};
    size_t i = 5;

    strerror ( EIO );

    fprintf ( stderr, EIO );


    getline_file ( buffer, f );
    printf ( "%s\n", buffer );

    my_getline_console ( buffer );
    printf ( "%s\n", buffer );

    my_getline_console ( buffer );
    printf ( "%s\n", buffer );

    //printf ( "size : %d\n", i );

    return 0;
}

int my_getline ( char *buffer, FILE *f )
{
    char temp[50] = {};
    int i = 0;
    int size = 0;

    //fscanf ( f, "%s", &temp );

    for ( int i = 0; strcmp ( temp, "\n" ) != 0; ) {
        ++size;
        fscanf ( f, "%s", &temp );
        strcat ( buffer, temp );
        strcat ( buffer, " " );
    }

    return size;
}

int getline_file ( char *buffer, FILE *fp )
{
    if ( !buffer ) {

        return -1;
    }

    int temp;
    size_t i = 0;

    while ( ( temp = fgetc ( fp ) ) != EOF && temp != '\n' ) {
        buffer[i++] = (char)temp;
    }

    if ( i > 0 || temp == '\n' ) {
        buffer[i] = '\0';

        return i;
    }

    return -1;
}

int my_getline_console ( char *buffer )
{
    if ( !buffer ) {

        return -1;
    }

    int temp;
    size_t i = 0;

    while ( ( temp = getchar ( ) ) != '\n' ) {
        buffer[i++] = (char)temp;
    }

    if ( i > 0 || temp == '\n' ) {
        buffer[i] = '\0';

        return i;
    }

    return -1;
}
