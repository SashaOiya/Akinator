#include "stack.h"

void StackCtor ( Stack_Data_t *Stack )
{
    int pointer = sizeof( canary_t ) / sizeof ( Node_t );

    Stack->data = (Node_t **)calloc( Stack->size_stack + 2 * pointer, sizeof ( Node_t* ) );
    assert ( Stack->data != nullptr );

    Stack->data = Stack->data + pointer;
}

void StackRealloc ( Stack_Data_t *Stack )
{
    Stack->size_stack = 2 * (Stack->size_stack);

    int pointer = sizeof( canary_t ) / sizeof ( Node_t );

$   Node_t **canary_begine = (Node_t **)calloc( Stack->size_stack + 2 * pointer , sizeof ( Node_t* ) );
    assert ( canary_begine != nullptr );

    Node_t **stack_begine = canary_begine + pointer;

$   memcpy ( stack_begine, Stack->data, Stack->size_stack * sizeof ( Node_t * ) / 2 );

    Stack->data = stack_begine;
}

void StackDump ( Stack_Data_t Stack, const char* func_name, const char* file_name )
{
$   printf ( "Stack [%p] ", Stack.data );
    name_print( Stack.data )
    printf ( " called from%s\n "
             "{\n\t%s  \n\t{ \n "
             " \t\tsize = %d \n "
             " \t\tcapacity = %d \n "
             " \t\tdata [%p]:\n", file_name, func_name, Stack.size_stack, Stack.capacity, Stack.data  );
$   for ( size_t i = 0; i < Stack.size_stack; ++i ) {
$       printf ( "\t\tdata[%d] = ", i );
        printf ( "%p", Stack.data[i] );
        printf ( "\n" );
    }
$   printf ( "\t}\n}\n");
}

void StackDtor ( Stack_Data_t *Stack )
{
$   memset ( Stack->data - sizeof ( canary_t ), 3, Stack->size_stack * sizeof ( Node_t )+ 2 * sizeof ( canary_t ) );

    free ( Stack->data - sizeof ( canary_t ) );
}

void StackPush ( Stack_Data_t *Stack, Node_t *value )
{
    assert ( Stack != nullptr );
    ++(Stack->capacity);

$   if ( Stack->capacity == Stack->size_stack ) {
$       StackRealloc ( Stack );
$   }
$   Stack->data[Stack->capacity - 1] = value;
}

Node_t StackPop ( Stack_Data_t *Stack  )
{
    Node_t temp = *( *Stack->data + Stack->capacity - 1 );
    --(Stack->capacity);
    *( (int *)Stack->data + Stack->capacity ) = 0; // const

    return temp;
}

