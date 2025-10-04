#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "stack_creation.h"
#include "error_handler.h"



/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackCtor ( Stack_t* stack, size_t capacity ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    assert ( stack != nullptr );

    stack->capacity = capacity;

    #ifdef DEBUG
    stack->size = 1;
    size_t alloc_size = capacity + 2;
    #else
    stack->size = 0;
    size_t alloc_size = capacity;
    #endif

    stack->data = (STK_ELM_TYPE*) calloc ( alloc_size, sizeof(STK_ELM_TYPE) );

    for ( size_t ind = 0; ind < capacity; ind++ ) stack->data[ind] = POISON_NUM;

    #ifdef DEBUG
    stack->data[0] = CANARY_NUM;
    stack->data[capacity + 1] = CANARY_NUM;
    #endif

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackDtor ( Stack_t* stack ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    assert ( stack != nullptr );

    stack->capacity = 0;
    stack->size = 0;
    
    free ( stack->data );
    stack->data = nullptr;

    return status;

}

/*-----------------------------------------------------------------------------------------------*/
