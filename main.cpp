#include "akin.h"

int main ()
{
    struct Tree_t tree = {};
    Stack_Data_t Stack = {};

    StackCtor ( &Stack );

    FILE *tree_f = fopen ( "tree.txt", "r" );  // argv // rw fseek
    if ( !tree_f ) {
        perror ( "File opening failed" );

        return ERR_FOPEN;
    }
    int akin_mode = 0;

    if ( File_Reader ( &tree.start, tree_f ) != OK_TREE ) {
         // fclose
        return EXIT_FAILURE;
    }

    do {
        akin_mode = interface_input ( );
        if ( akin_mode == MODE_START ) {
            if ( Akinator ( &tree.start, &Stack ) ) {

                return EXIT_FAILURE;
            }
            Tree_Verificator ( tree.start );
            reverseArray ( Stack.data, Stack.capacity );
        }
        else if ( akin_mode == MODE_DEFINE ) {
            Akinator_Definition ( tree.start, &Stack );
        }
    } while ( akin_mode != MODE_BYE );

    free ( tree_f );

    tree_f = fopen ( "tree.txt", "w" );
    if ( !tree_f ) {
        perror ( "File opening failed" );

        return ERR_FOPEN;
    }

    File_Write_Node ( tree.start, tree_f );
    Tree_Graph_Dump ( tree.start );

$   Tree_Text_Dump ( tree.start );

    fclose ( tree_f );

    Tree_Dtor ( tree.start );

    return 0;
}

Errors_t File_Reader ( struct Node_t **tree, FILE * f ) // *tree?
{
    const int size = 100;  // getfilesize
    char data[size] = {};

    int n_arg = 0;

    if ( ( n_arg = my_getline_file ( data, f ) ) == 0 || n_arg == EOF ) {

        return ERR_FREAD;
    }
    if ( *data == ')' ) {  //
        n_arg = my_getline_file ( data, f );
    }
    if ( *data == '(' ) {  //
        *tree = (Node_t *)calloc ( 1, sizeof ( Node_t ) );
        if ( !(*tree) ) {
            free ( *tree );

            return ERR_CALLO;
        }
$       n_arg = my_getline_file ( data, f );
    }
    if ( n_arg == 0 || n_arg == EOF || n_arg > size ) {
        (*tree)->err_gauge =   ( (*tree)->err_gauge ) |
                             ( ( (*tree)->err_gauge | 1 ) << ERR_FREAD );

        return ERR_FREAD;
    }
    if ( strcmp ( data, "\0" )  == 0 ||
         strcmp ( data, "nil" ) == 0 ) {

        return OK_TREE;
    }
    ++((*tree)->count);
$   (*tree)->data = strdup( data );

$   File_Reader ( &((*tree)->left) , f ); //?????  //
    File_Reader ( &((*tree)->right), f ); //

    n_arg = my_getline_file ( data, f );

    if ( strcmp ( data, ")" ) != 0 || n_arg == ERR_RLINE ) {
        printf ( "ERROR\n" );

        return ERR_FREAD;
    }

    return OK_TREE;
}

void Tree_Text_Dump ( const struct Node_t *tree_node )
{

    if ( tree_node == nullptr) {
        printf ( " nil " );

        return ;
    }
    printf ( " ( " );
    printf ( "%s", tree_node->data );

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
    fprintf ( tree_f, "%s", tree_node->data );
    fprintf ( tree_f, "\n"  );

    File_Write_Node ( tree_node->left,  tree_f );
    File_Write_Node ( tree_node->right, tree_f );

    fprintf ( tree_f,  ")\n" );

}

Errors_t Akinator ( struct Node_t **tree, struct Stack_Data_t *Stack )
{
    print_color ( (*tree)->data, COLOR_YELLOW );
    print_color ( "?\n", COLOR_YELLOW );

    char answer[10] = {};
    my_getline_console ( answer );

    if ( strcmp ( answer, "yes" ) == 0 ) {
        if ( (*tree)->right == nullptr )  {
            print_color ( "Stupid man, think of something more complicated! BUGAGA\n", COLOR_GREEN );
            //txSpeak ( "Stupid man, think of something more complicated! BUGAGA\n" );

            return OK_TREE;
        }
        else if ( (*tree)->right != nullptr ) {
            StackPush ( Stack, YES );
            Akinator ( &(*tree)->right, Stack );
        }
    }
    else if ( strcmp ( answer, "no" ) == 0 ) {
        if ( (*tree)->left == nullptr ) {
            print_color ( "Enter your answer option\n", COLOR_BLUE );

            char true_answer[100] = {};
$           if ( !my_getline_console ( true_answer ) ) {
                printf ( "Input error\n" );

                return ERR_INPUT;
            }

            print_color ( "How does %s differ from %s?\n", COLOR_BLUE, true_answer, (*tree)->data );

            char difference[100] = {};
$           while ( !my_getline_console ( difference ) ) {  //
                printf ( "Input error\n" );

                return ERR_INPUT;
            }

            (*tree)->left  = (Node_t *)calloc ( 1, sizeof ( Node_t ) );
            (*tree)->right = (Node_t *)calloc ( 1, sizeof ( Node_t ) );
            if ( !( (*tree)->right && (*tree)->left ) ) {
                free ( (*tree)->right );
                free ( (*tree)->left  );

                return ERR_CALLO;
            }

            char *temp = (*tree)->data;

$           ((*tree)->left)->data  = strdup ( temp );
            ((*tree)->right)->data = strdup( true_answer );
$           (*tree)->data          = strdup ( difference );

            print_color ( "Now I know who it is. You can't beat me now \n", COLOR_GREEN );

            return OK_TREE;
        }
        else {
            StackPush ( Stack, NO );
            Akinator ( &(*tree)->left, Stack );
        }
    }
    else {
        print_color ( "Sorry, but you have to answer \"yes\" or \"no\"", COLOR_RED );

        return ERR_INPUT;
    }

    return OK_TREE;
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
                         " \"%p\" ", tree );

    Tree_Dump_Body ( tree, tree_dump );

    fprintf ( tree_dump, "}\n" );

    //system ( "del list.png" );
    /*system ( "dot -T png tree.dot -o tree.png" );
    system ( "tree.png" );  */

    fclose ( tree_dump );

    return OK_TREE;
}

void Tree_Dump_Body ( const struct Node_t *tree, FILE *tree_dump )
{
    if ( tree == nullptr) {

        return ;
    }
    fprintf ( tree_dump , " \"%p\" [shape = Mrecord, style = filled, fillcolor = lightpink "
                          " label = \"data: %s \"];\n",tree, tree->data );
    if ( tree->left != nullptr ) {
        fprintf ( tree_dump, "\"%p\" -> \"%p\" ", tree, tree->left );
    }
    if ( tree->right != nullptr ) {
        fprintf ( tree_dump, "\n \"%p\" -> \"%p\" \n", tree, tree->right );
    }

    Tree_Dump_Body ( tree->left,  tree_dump );
    Tree_Dump_Body ( tree->right, tree_dump );
}

Errors_t Tree_Verificator ( struct Node_t *tree )
{
    if ( tree == nullptr ) {

        return OK_TREE;
    }
    if ( tree->count != TREE_ONE_CALL ) {
        printf ( "There is a loop or an unused node \n" );
        tree->err_gauge = ( tree->err_gauge ) | ( ( tree->err_gauge | 1 ) << ERR_CYCLE );

        return ERR_CYCLE;
    }
    if ( ( ( tree->err_gauge >> ERR_FREAD ) & 1 ) == 1 ) {
        printf ( "Read error\n" );

        return ERR_FREAD;
    }
    Tree_Verificator ( tree->left  );
    Tree_Verificator ( tree->right );

    return OK_TREE;
}

void Akinator_Definition ( const struct Node_t *tree, struct Stack_Data_t *stack )
{
    if ( tree == nullptr || stack->capacity == 0 ) {

        return;
    }
    if ( StackPop ( stack ) == YES ) {
        printf ( "%s ", tree->data );

        Akinator_Definition ( tree->right, stack );
    }
    else if ( StackPop ( stack ) == NO ) {

        Akinator_Definition ( tree->left, stack );
    }

}

void reverseArray ( int arr[], int size ) {
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

Mode_t interface_input ( )
{
$   const int max_buf_value = 100;
$   char buf[max_buf_value] = {};

    print_color ( "Select mode\n", COLOR_BLUE );
    scanf ( "%s", buf );
    getchar ( );

    if ( strcmp ( buf, "start" ) == 0 ) {  // &&   start

        return MODE_START;
    }
    else if ( strcmp( buf, "help" ) == 0 ) {

        print_color ( "Here is a list of supported features :"
                      "\n\n  start  \n\n  help  \n\n  bye  \n\n  definition  \n\n  dump \n\n", COLOR_BLUE );

$       return MODE_HELP;
    }
    else if ( strcmp ( buf, "bye" ) == 0 ) {

        return MODE_BYE;
    }
    else if ( strcmp ( buf, "definition" ) == 0 ) {

        return MODE_DEFINE;
    }
    else if ( strcmp ( buf, "dump" ) == 0 ) {
        system ( "dot -T png tree.dot -o tree.png" );
        system ( "tree.png" );

        return MODE_DUMP;
    }
    else {

        print_color ( "This option was not found. Use the list of presented functions:\n", COLOR_RED );

        return MODE_ERROR;
    }

    return MODE_ERROR;
}
