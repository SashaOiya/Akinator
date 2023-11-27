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

struct Node_t {
    elem_t data  = 0;
    Node_t *left   = nullptr;
    Node_t *right  = nullptr;
    int count    = 0;  // chislo vhogdeniy
};

struct Tree_t {
    Node_t *start = nullptr;
    //Array
    //size_t capacity = 5;
};

enum Errors_t {
    OK        = 0,
    ERR_FREAD = 1,
    ERR_INPUT = 2,
    ERR_FOPEN = 3,
    ERR_RLINE = 4,
    ERR_CALLO = 5
};

const int DEFAULT_VALUE = 1; // choooooooooooooooooooooooooooooooo???

Errors_t File_Reader     ( struct Node_t **tree, FILE * f );
Errors_t Akinator        ( struct Node_t **tree );
void     Tree_Dtor       ( struct Node_t *tree );

void     Tree_Text_Dump  ( const struct Node_t *tree_knot );
Errors_t Tree_Graph_Dump ( const struct Node_t *tree );
void     Tree_Dump_Body  ( const struct Node_t *tree, FILE *tree_dump );
void     File_Write_Node ( const struct Node_t *tree_knot, FILE *tree );

int my_getline_file    ( char *buffer, FILE *f  );
int my_getline_console ( char *buffer );

int main ()
{
    struct Tree_t tree = {};
    FILE *tree_f = fopen ( "tree.txt", "r" );
    if ( !tree_f ) {
        perror ( "File opening failed" );

        return ERR_FOPEN;
    }

    if ( File_Reader ( &tree.start, tree_f ) != OK ) {

        return EXIT_FAILURE;
    }

    if ( Akinator ( &tree.start ) ) {

        return EXIT_FAILURE;
    }
    free ( tree_f );

    FILE *tree2_f = fopen ( "tree.txt", "w" );
    if ( !tree2_f ) {
        perror ( "File opening failed" );

        return ERR_FOPEN;
    }

    File_Write_Node ( tree.start, tree2_f );
    Tree_Graph_Dump ( tree.start );

$   Tree_Text_Dump ( tree.start );

    fclose ( tree_f );

    Tree_Dtor ( tree.start );

    return 0;
}

Errors_t File_Reader ( struct Node_t **tree, FILE * f )
{
    const int size = 100;
    char data[size] = {};

    int error_indicator = DEFAULT_VALUE;

    if ( ( error_indicator = my_getline_file ( data, f ) ) == 0 || error_indicator == EOF ) {

        return ERR_FREAD;
    }
    if ( strcmp ( data, ")" ) == 0 ) {
        error_indicator = my_getline_file ( data, f );
    }
    if ( strcmp ( data, "(" ) == 0 ) {  // strncmp
        *tree = (Node_t *)calloc ( 1, sizeof ( Node_t ) );
        if ( !(*tree) ) {
            free ( *tree );

            return ERR_CALLO;
        }
$       error_indicator = my_getline_file ( data, f );
    }
    if ( error_indicator < DEFAULT_VALUE ) {  //????

        return ERR_FREAD;
    }
    if ( strcmp ( data, "\0" )  == 0 ||
         strcmp ( data, "nil" ) == 0 ) {

        return OK;
    }
$   (*tree)->data = strdup( data );

$   File_Reader ( &((*tree)->left) , f ); //?????  //
    File_Reader ( &((*tree)->right), f ); //

    error_indicator = my_getline_file ( data, f );

    if ( strcmp ( data, ")" ) != 0 || error_indicator == ERR_RLINE ) {
        printf ( "ERROR\n" );

        return ERR_FREAD;
    }

    return OK;
}

void Tree_Text_Dump ( const struct Node_t *tree_node )
{

    if ( tree_node == nullptr) {
        printf ( " nil " );

        return ;
    }
    printf ( " ( " );
    printf ( SPECIFIER, tree_node->data );

    Tree_Text_Dump ( tree_node->left  );
    Tree_Text_Dump ( tree_node->right );

    printf ( " ) " );

}

void File_Write_Node  ( const struct Node_t *tree_node, FILE *tree_f )
{
    if ( tree_node == nullptr ) {
        fprintf ( tree_f, "nil\n" );

        return ;
    }
    fprintf ( tree_f, "(\n" );
    fprintf ( tree_f, SPECIFIER, tree_node->data );
    fprintf ( tree_f, "\n"  );

    File_Write_Node ( tree_node->left,  tree_f );
    File_Write_Node ( tree_node->right, tree_f );

    fprintf ( tree_f,  ")\n" );

}

Errors_t Akinator ( struct Node_t **tree )
{
    printf ( SPECIFIER, (*tree)->data );
    printf ( "?\n" );

    char answer[10] = {};
    my_getline_console ( answer );

    if ( strcmp ( answer, "yes" ) == 0 ) {
        if ( (*tree)->right == nullptr )  {
            printf ( "Stupid man, think of something more complicated! BUGAGA\n" );
            txSpeak ( "Stupid man, think of something more complicated! BUGAGA\n" );

            return OK;
        }
        else if ( (*tree)->right != nullptr ) {
            Akinator ( &(*tree)->right );
        }
    }
    else if ( strcmp ( answer, "no" ) == 0 ) {
        if ( (*tree)->left == nullptr ) {
            printf ( "Enter your answer option\n" );

            char true_answer[100] = {};
$           if ( !my_getline_console ( true_answer ) ) {
                printf ( "Input error\n" );

                return ERR_INPUT;
            }

            printf ( "How does %s differ from %s? ?\n", true_answer, (*tree)->data );

            char difference[100] = {};
$           if ( !my_getline_console ( difference ) ) {
                printf ( "Input error\n" );

                return ERR_INPUT;
            }

$           char *temp = (*tree)->data;

            (*tree)->left  = (Node_t *)calloc ( 1, sizeof ( Node_t ) );
            (*tree)->right = (Node_t *)calloc ( 1, sizeof ( Node_t ) );
            if ( !( (*tree)->right && (*tree)->left ) ) {
                free ( (*tree)->right );
                free ( (*tree)->left  );

                return ERR_CALLO;
            }

$           ((*tree)->left)->data  = strdup ( temp );
            ((*tree)->right)->data = strdup( true_answer );
$           (*tree)->data          = strdup ( difference );

            printf ( "Now I know who it is. You can't beat me now \n" );

            return OK;
        }
        else {
            Akinator ( &(*tree)->left );
        }
    }
    else {
        printf ( "Sorry, but you have to answer \"yes\" or \"no\"" );

        return ERR_INPUT;
    }

    return OK;
}

void Tree_Dtor ( struct Node_t *tree )
{
    if ( tree != nullptr ) {
        Tree_Dtor ( tree->left  );
        Tree_Dtor ( tree->right );

        free ( tree );
    }
}

Errors_t Tree_Graph_Dump ( const struct Node_t *tree )
{
    FILE *tree_dump = fopen ( "tree.dot", "w" );
    if ( !tree_dump ) {
        perror ( "File opening failed" );

        return ERR_FOPEN;
    }

    fprintf ( tree_dump, "digraph G { \n"
                         "node [shape = record];\n"
                         " %o ", tree );

    Tree_Dump_Body ( tree, tree_dump );

    fprintf ( tree_dump, "}\n" );

    //system ( "del list.png" );
    /*system ( "dot -T png tree.dot -o tree.png" );
    system ( "tree.png" );  */

    fclose ( tree_dump );

    return OK;
}

void Tree_Dump_Body ( const struct Node_t *tree, FILE *tree_dump )
{
    if ( tree == nullptr) {

        return ;
    }
    fprintf ( tree_dump , " %o [shape = Mrecord, style = filled, fillcolor = lightpink "
                          " label = \"data: %s \"];\n",tree, tree->data );
    if ( tree->left != nullptr ) {
        fprintf ( tree_dump, "\"%o\" -> %o", tree, tree->left );
    }
    if ( tree->right != nullptr ) {
        fprintf ( tree_dump, "\n %o -> %o \n", tree, tree->right );
    }

    Tree_Dump_Body ( tree->left,  tree_dump );
    Tree_Dump_Body ( tree->right, tree_dump );
}

int my_getline_file ( char *buffer, FILE *fp )
{
    if ( !buffer ) {

        return ERR_RLINE;
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

    return ERR_RLINE;
}

int my_getline_console ( char *buffer )
{
    if ( !buffer ) {

        return ERR_RLINE;
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

    return ERR_RLINE;
}




