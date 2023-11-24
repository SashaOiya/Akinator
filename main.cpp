#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TX_SPEAK

#ifdef TX_SPEAK
    #include <TXLib.h>
#endif

typedef char* elem_t;
#define SPECIFIER "%s"

#ifdef DEBUGG
#define $ printf ( "function <%s> line <%d>\n ", __PRETTY_FUNCTION__, __LINE__ );
#else
#define $
#endif

struct Knot {
    elem_t data  = 0;
    Knot *left   = 0;
    Knot *right  = 0;
    int count    = 0;  // chislo vhogdeniy
};

struct Tree_t {
    Knot *start = 0;
    //size_t capacity = 5;
};

enum Errors_t {
    ERR_NO   = 0,
    ERR_READ = 1,
    ERR_FILE = 2
};

void     Tree_Text_Dump  ( const struct Knot *tree_knot );
void     File_Write_Node ( const struct Knot *tree_knot, FILE *tree );
Errors_t File_Reader     ( FILE * f, struct Knot **tree );
void     Akinator        ( struct Knot **tree );
void     Tree_Dtor       ( struct Knot *tree );
void     Tree_Dump_Link  ( FILE *tree_dump, const struct Knot *tree );
void     Tree_Graph_Dump ( const struct Knot *tree );
void     Tree_Dump_Body  ( FILE *tree_dump, const struct Knot *tree );

int main ()
{
    struct Tree_t tree = {};
    FILE *tree_f = fopen ( "tree.txt", "r+" );

    File_Reader ( tree_f, &tree.start );

    Akinator ( &tree.start );

    fclose ( tree_f );

    FILE *tree2_f = fopen ( "tree.txt", "w" );

    File_Write_Node ( tree.start, tree2_f );
    Tree_Graph_Dump ( tree.start );

$   Tree_Text_Dump ( tree.start );

    fclose ( tree_f );

    Tree_Dtor ( tree.start );

    return 0;
}

Errors_t File_Reader ( FILE * f, struct Knot **tree )
{
    char word[100] = {};
    //int size = 100;

    fscanf ( f, "%s", &word );
    //getdelim ( &word, &size, "\n", f );
    //getline  ();

    if ( strcmp ( word, ")" ) == 0 ) {    //switch
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
$   (*tree)->data = strdup( word );

$   File_Reader ( f, &((*tree)->left) );
    File_Reader ( f, &((*tree)->right) );

    fscanf ( f, "%s", &word ); //getline

    if ( strcmp ( word, ")" ) != 0 ) {
        printf ( "ERROR\n" );

        return ERR_READ;
    }

    return ERR_NO;
}

void Tree_Text_Dump ( const struct Knot *tree_knot )
{

    if ( tree_knot == nullptr) {
        printf     ( " nil " );

        return ;
    }
    printf         ( " ( " );
    printf         ( SPECIFIER, tree_knot->data );

    Tree_Text_Dump ( tree_knot->left );
    Tree_Text_Dump ( tree_knot->right );

    printf         ( " ) " );

}

void File_Write_Node  ( const struct Knot *tree_knot, FILE *tree_f )
{
    if ( tree_knot == nullptr) {
        fprintf     ( tree_f, "nil\n" );

        return ;
    }
    fprintf         ( tree_f, "(\n" );
    fprintf         ( tree_f, SPECIFIER, tree_knot->data );
    fprintf         ( tree_f, "\n" );

    File_Write_Node ( tree_knot->left, tree_f );
    File_Write_Node ( tree_knot->right, tree_f );

    fprintf         ( tree_f,  ")\n" );

}

void Akinator ( struct Knot **tree )
{
    printf ( SPECIFIER, (*tree)->data );
    printf ( "?\n" );

    char answer[10] = {};
    scanf ( "%s", answer );

    if ( strcmp ( answer, "yes" ) == 0 ) {
        printf ( "Stupid man, think of something more complicated! BUGAGA\n" );
        txSpeak ( "Stupid man, think of something more complicated! BUGAGA\n" );
    }
    else if ( strcmp ( answer, "no" ) == 0 ) {
        if ( (*tree)->left == nullptr ) {
            printf ( "WHO ?\n" );

            char word[100] = {};

            (*tree)->left = (Knot *)calloc ( 1, sizeof ( Knot ) );
$           scanf ( "%s", &word );
            ((*tree)->left)->data = strdup( word );

            printf ( "Now I know who it is. You can't beat me now \n" );

            return ;
        }
        Akinator ( &(*tree)->left );
    }
}

void Tree_Dtor ( struct Knot *tree )
{
    if ( tree != nullptr ) {
        Tree_Dtor ( tree->left  );
        Tree_Dtor ( tree->right );

        free ( tree );
    }

    return ;
}

void Tree_Graph_Dump ( const struct Knot *tree )
{
    FILE *tree_dump = fopen ( "tree.dot", "w" );

    fprintf ( tree_dump, "digraph G { \n"
                         "rankdir = LR;\n"
                         "node [shape = record];\n"
                         " %o ", tree );

    Tree_Dump_Link ( tree_dump, tree );

    fprintf ( tree_dump, "[arrowsize = 0.0, weight = 10000, color = \"#FFFFFF\"];\n" );

    Tree_Dump_Body ( tree_dump, tree );

    fprintf ( tree_dump, "}\n" );

    //system ( "del list.png" );
    /*system ( "dot -T png tree.dot -o tree.png" );
    system ( "tree.png" );  */

    fclose ( tree_dump );
}

void Tree_Dump_Link ( FILE *tree_dump, const struct Knot *tree )
{
    if ( tree == nullptr) {

        return ;
    }
    fprintf ( tree_dump, "-> %o", tree );

    Tree_Dump_Link ( tree_dump, tree->left );
    Tree_Dump_Link ( tree_dump, tree->right );
}

void Tree_Dump_Body ( FILE *tree_dump, const struct Knot *tree )
{
    if ( tree == nullptr) {

        return ;
    }
    fprintf ( tree_dump , " %o [shape = Mrecord, style = filled, fillcolor = lightpink "
                          " label = \"data: %s | right: %s | left: %s\"];\n ",tree, tree->data,
                                                                              tree->right,
                                                                              tree->left  );
    if ( tree->left != nullptr ) {
        fprintf ( tree_dump, "%o -> %o", tree, tree->left );
    }
    if ( tree->right != nullptr ) {
        fprintf ( tree_dump, "\n %o -> %o \n", tree, tree->right );
    }

    Tree_Dump_Body ( tree_dump, tree->left );
    Tree_Dump_Body ( tree_dump, tree->right );
}


