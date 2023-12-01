#ifndef AKIN
#define AKIN

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "stack.h"

//#define TX_SPEAK

#ifdef TX_SPEAK
    #include <TXLib.h>
#endif

const int TREE_ONE_CALL = 1;

enum Define_Stack {
    NO  = 1,
    YES = 2
};

#ifdef DEBUGG
#define $ printf ( "function <%s> line <%d>\n ", __PRETTY_FUNCTION__, __LINE__ );
#else
#define $
#endif

struct Node_t {
    char* data    = 0;
    Node_t *left   = nullptr;
    Node_t *right  = nullptr;
    int   count    = 0;  // chislo vhogdeniy
    int err_gauge  = 0;
};

enum Mode_t {
    MODE_START = 1,
    MODE_AKIN  = 2,
    MODE_DEFINE = 3,
    MODE_BYE    = 4,
    MODE_ERROR  = 5,
    MODE_HELP   = 6
};

struct Tree_t {
    Node_t *start = nullptr;
    //Array
};

enum Color_t {
    COLOR_GREEN  = 10,
    COLOR_WHITE  = 15,
    COLOR_RED    =  4,
    COLOR_PINK   = 12,
    COLOR_BLUE   =  9,
    COLOR_YELLOW = 14
};

enum Errors_t {
    OK_TREE   = 0,
    ERR_FREAD = 1,
    ERR_INPUT = 2,
    ERR_FOPEN = 3,
    ERR_RLINE = 4,
    ERR_CALLO = 5,
    ERR_CYCLE = 6
};

Errors_t File_Reader ( struct Node_t **tree, FILE * f );
Errors_t Akinator ( struct Node_t **tree, struct Stack_Data_t *Stack );
void     Akinator_Definition  ( const struct Node_t *tree, struct Stack_Data_t *stack );
void     Tree_Dtor   ( struct Node_t *tree );

void     Tree_Text_Dump       ( const struct Node_t *tree_knot );
Errors_t Tree_Graph_Dump      ( const struct Node_t *tree );
Errors_t Tree_Verificator     ( struct Node_t *tree );
void     Tree_Dump_Body       ( const struct Node_t *tree, FILE *tree_dump );
void     File_Write_Node      ( const struct Node_t *tree_knot, FILE *tree );

Mode_t interface_input ( );

int  my_getline_file    ( char *buffer, FILE *f  );
int  my_getline_console ( char *buffer );
void print_color       ( const char *line, enum Color_t COLOR, ... );
void reverseArray ( int arr[], int size );
void buf_input ( char *buf, const int max_buf_value );

#endif      // AKIN
