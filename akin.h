#ifndef AKIN
#define AKIN

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "stack.h"
#include "my_prog.h"
#include "akin_log.h"

//#define TX_SPEAK

#ifdef TX_SPEAK
    #include <TXLib.h>
#endif

const int TREE_ONE_CALL = 1; // global why???

enum Define_Stack {
    NO  = 1,
    YES = 2
};

struct Node_t {
    char* data     = 0;
    Node_t *left   = nullptr;
    Node_t *right  = nullptr;
    int   count    = 0;  // chislo vhogdeniy    debug_jkjkkjkj
    int err_gauge  = 0;
};

enum Mode_t {
    MODE_START  = 1,
    MODE_AKIN   = 2,
    MODE_DEFINE = 3,
    MODE_BYE    = 4,
    MODE_ERROR  = 5,
    MODE_HELP   = 6,
    MODE_DUMP   = 7
};

struct Tree_t {
    Node_t *start = nullptr;
    //Array
};

Errors_t File_Reader ( struct Node_t **tree, FILE * f );
Errors_t Akinator( struct Node_t **tree, struct Stack_t *Stack );  //????????
void Akinator_Definition( const struct Node_t *tree, struct Stack_t *stack );
void Akin_Tree_Dtor( struct Tree_t *tree );
void Node_Free ( struct Node_t **tree_node );

void     Tree_Text_Dump( const struct Node_t *tree_knot );
Errors_t Tree_Graph_Dump( const struct Node_t *tree );
Errors_t Tree_Verificator( struct Node_t *tree );
void     Tree_Dump_Body       ( const struct Node_t *tree, FILE *tree_dump );
void     File_Write_Node      ( const struct Node_t *tree_knot, FILE *tree );

Mode_t interface_input ( );
void buf_input ( char *buf, const int max_buf_value );

#endif      // AKIN
