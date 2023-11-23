#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef char* elem_t;
#define SPECIFIER "%s"

#ifdef DEBUGG
#define $ printf ( "function <%s> line <%d>\n ", __PRETTY_FUNCTION__, __LINE__ );
#else
#define $
#endif

struct Knot {
    elem_t value = 0;  // name //calloc
    Knot *left   = 0;
    Knot *right  = 0;
    int count = 0;  // chislo vhogdeniy
};

struct Tree_t {
    Knot *start = 0;
    //size_t capacity = 5;
};

enum Errors_t {
    ERR_NO   = 0,
    ERR_READ = 1

};

void Print_Node  ( const struct Knot *tree_knot );
void File_Write_Node  ( const struct Knot *tree_knot, FILE *tree );
Errors_t File_Reader ( FILE * f, struct Knot **tree );
void Graph_Dump  ( const struct Akin_t *tree );

int main ()
{
    struct Tree_t tree = {};
    FILE *tree_f = fopen ( "tree.txt", "r+" );

    File_Reader ( tree_f, &tree.start );

    File_Write_Node  ( tree.start, tree_f );

    Print_Node ( tree.start );

    fclose ( tree_f );

    return 0;
}

Errors_t File_Reader ( FILE * f, struct Knot **tree )
{
    char word[100] = {};
    fscanf ( f, "%s", &word ); //getline

    if ( strcmp ( word, ")" ) == 0 ) {
        fscanf ( f, "%s", &word );
    }
    if ( strcmp ( word, "(" ) == 0 ) {
        *tree = (Knot *)calloc ( 1, sizeof ( Knot ) );
$       fscanf ( f, "%s", &word );
    }
    if ( strcmp ( word, "\0" ) == 0 ||
         strcmp ( word, "nil" ) == 0 ) {

        return ERR_NO;
    }
$   (*tree)->value = strdup( word );

$   File_Reader ( f, &((*tree)->left) );
    File_Reader ( f, &((*tree)->right) );

    fscanf ( f, "%s", &word ); //getline

    if ( strcmp ( word, ")" ) != 0 ) {
        printf ( "ERROR\n" );

        return ERR_READ;
    }

    return ERR_NO;
}

void Print_Node  ( const struct Knot *tree_knot )
{

    if ( tree_knot == nullptr) {
        printf ( " nil " );

        return ;
    }
    printf ( " ( " );

    printf ( SPECIFIER, tree_knot->value );

    Print_Node ( tree_knot->left );
    Print_Node ( tree_knot->right );

    printf ( " ) " );

}

void File_Write_Node  ( const struct Knot *tree_knot, FILE *tree_f )
{
    //FILE *tree = fopen ( "tree.txt", "w" );

    if ( tree_knot == nullptr) {
        fprintf ( tree_f, " nil " );

        return ;
    }
    fprintf ( tree_f, " ( \n" );

    fprintf ( tree_f, SPECIFIER, tree_knot->value );
    fprintf ( tree_f, "\n" );

    File_Write_Node ( tree_knot->left, tree_f );
    File_Write_Node ( tree_knot->right, tree_f );

    fprintf ( tree_f,  " ) \n" );

}

/*void Akinator () {
    while ( tree.data[ip].value != nullptr ) {
        printf ( SPECIFIER, tree.data[ip].value );
        printf ( "?\n" );

        char answer[10] = {};
        scanf ( "%s", answer );

        if ( strcmp ( answer, "yes" ) == 0 ) {
            ip = tree.data[ip].right->ip;
            //printf ( SPECIFIER, tree.data[ip].value );
        }
        else if ( strcmp ( answer, "no" ) == 0 ) {
            ip = tree.data[ip].left->ip;
            //printf ( SPECIFIER, tree.data[ip].value );
        }
    }
}   */

//void File_Read_Node  ( const struct Knot *tree_knot, FILE *tree ) {


