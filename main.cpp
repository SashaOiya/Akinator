#include "akin.h"

static const int DUMP_COUNTER = 100;

int main ()     // srazy chackat dump new
{
    struct Tree_t tree = {};
    Stack_t Stack = {};

    Stack_Ctor ( &Stack );

    FILE *tree_f = fopen ( "tree.txt", "r" );  // argv // rw fseek
    if ( !tree_f ) {
        perror ( "File opening failed" );

        return ERR_FOPEN;
    }
    int akin_mode = 0;

    if ( File_Reader ( &tree.start, tree_f ) != NO_ERR ) {
         // fclose
        return EXIT_FAILURE;
    }

    do {
        akin_mode = interface_input ( );
        if ( akin_mode == MODE_START ) {
            if ( Akinator ( &tree.start, &Stack ) ) {

                return EXIT_FAILURE;
            }
            //Tree_Verificator ( tree.start );
            reverse_array ( Stack.data, Stack.capacity );
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

    Akin_Tree_Dtor ( &tree );

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

        return NO_ERR;
    }
    ++((*tree)->count);
$   (*tree)->data = strdup( data );

$   File_Reader ( &((*tree)->left) , f ); //?????  //
    File_Reader ( &((*tree)->right), f ); //

    n_arg = my_getline_file ( data, f );

    if ( *data != ')' || n_arg == ERR_RLINE ) {
        printf ( "ERROR\n" );

        return ERR_FREAD;
    }

    return NO_ERR;
}

void Tree_Text_Dump ( const struct Node_t *tree_node )   // +
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

void File_Write_Node  ( const struct Node_t *tree_node, FILE *tree_f )  // change file* to char*
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

Errors_t Akinator ( struct Node_t **tree, struct Stack_t *Stack )  // little letter
{
    assert ( tree != nullptr );
    assert ( Stack != nullptr );

    print_color ( (*tree)->data, COLOR_YELLOW );
    print_color ( "?\n", COLOR_YELLOW );

    char answer[10] = {};
    my_getline_console ( answer );   // weird

    if ( strcmp ( answer, "yes" ) == 0 ) {
        if ( (*tree)->right == nullptr )  {
            print_color ( "Stupid man, think of something more complicated! BUGAGA\n", COLOR_GREEN );

            return NO_ERR;
        }
        else if ( (*tree)->right != nullptr ) {
            Stack_Push ( Stack, YES );    //
            Akinator ( &(*tree)->right, Stack );
        }
    }
    else if ( strcmp ( answer, "no" ) == 0 ) {          // new function
        if ( (*tree)->left == nullptr ) {
            print_color ( "Enter your answer option\n", COLOR_BLUE );       //  separate func
                                                                            //
            char true_answer[100] = {};                                     //
$           while ( !my_getline_console ( true_answer ) ) {                 //
                printf ( "Incorrectly entered value. Please, try again\n" );//
            }                                                               //

            print_color ( "How does %s differ from %s?\n", COLOR_BLUE, true_answer, (*tree)->data );   //
                                                                                                       //
            char difference[100] = {};                                                                 //
$           while ( !my_getline_console ( difference ) ) {                                             //
                printf ( "Incorrectly entered value. Please, try again\n" );                           //
            }                                                                                          //

            (*tree)->left  = (Node_t *)calloc ( 1, sizeof ( Node_t ) );   // for this
            (*tree)->right = (Node_t *)calloc ( 1, sizeof ( Node_t ) );
            if ( !( (*tree)->right && (*tree)->left ) ) {
                free ( (*tree)->left  );
                free ( (*tree)->right );

                print_color ( "Buffer memory allocation error."
                              "Please, try again \n", COLOR_RED );

                Akinator ( tree, Stack );
            }

$           ((*tree)->left)->data  = strdup ( (*tree)->data );
            ((*tree)->right)->data = strdup ( true_answer );
$           (*tree)->data          = strdup ( difference );

            print_color ( "Now I know who it is. You can't beat me now \n", COLOR_GREEN );

            return NO_ERR;
        }
        else {
            Stack_Push ( Stack, NO );    //
            Akinator ( &(*tree)->left, Stack );
        }
    }
    else {
        print_color ( "Sorry, but you have to answer \"yes\" or \"no\"\n", COLOR_RED );
        Akinator ( tree, Stack );
    }

    return NO_ERR;
}

void Akin_Tree_Dtor ( struct Tree_t *tree ) //struct File_t *file ) // +
{
    Node_Free ( &(tree->start) );
    //free ( file->out_buffer );
    //fclose ( file->front_f );
}

void Node_Free ( struct Node_t **tree_node ) // +
{
    if ( tree_node != nullptr && *tree_node != nullptr ) {
        Node_Free ( &((*tree_node)->left) );
        Node_Free ( &((*tree_node)->right));

        (*tree_node)->left = nullptr;
        (*tree_node)->right = nullptr;
        free ( (*tree_node)->left );
        free ( (*tree_node)->right );

        *tree_node = nullptr;
    }
}

Errors_t Tree_Graph_Dump ( const struct Node_t *tree ) // +
{
    assert ( tree != nullptr );

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

    static int file_counter = 0;
    char command_buffer[DUMP_COUNTER] = {};
    fprintf( log(), "<img src=\"tree%d.png\" alt=\"-\" width=\"500\" height=\"600\">\n", file_counter );
    sprintf( command_buffer, "dot -T png tree.dot -o logs/tree%d.png", file_counter++ );
    system ( command_buffer );

    return NO_ERR;
}

void Tree_Dump_Body ( const struct Node_t *tree, FILE *tree_dump_f ) // +
{
    assert ( tree_dump_f != nullptr );

    if ( tree == nullptr) {

        return ;
    }
    fprintf ( tree_dump_f , " \"%p\" [shape = Mrecord, style = filled, fillcolor = lightpink "
                          " label = \"data: %s \"];\n",tree, tree->data );
    if ( tree->left != nullptr ) {
        fprintf ( tree_dump_f, "\"%p\" -> \"%p\" ", tree, tree->left );
    }
    if ( tree->right != nullptr ) {
        fprintf ( tree_dump_f, "\n \"%p\" -> \"%p\" \n", tree, tree->right );
    }

    Tree_Dump_Body ( tree->left,  tree_dump_f );
    Tree_Dump_Body ( tree->right, tree_dump_f );
}

Errors_t Tree_Verificator ( struct Node_t *tree ) // hyita
{
    if ( tree == nullptr ) {

        return NO_ERR;
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

    return NO_ERR;
}

void Akinator_Definition ( const struct Node_t *tree, struct Stack_t *stack )  // hyita
{
    if ( tree == nullptr || stack->capacity == 0 ) {

        return;
    }
    if ( Stack_Pop ( stack ) == YES ) {
        print_color ( tree->data, COLOR_PINK );

        Akinator_Definition ( tree->right, stack );
    }
    else if ( Stack_Pop ( stack ) == NO ) {

        Akinator_Definition ( tree->left, stack );
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
