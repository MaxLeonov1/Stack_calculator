#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "error_handler.h"
#include "stack_creation.h"
#include "calc_comands.h"
#include "colors.h"
/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackPush ( Stack_t* stack, STK_ELM_TYPE value ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    assert ( stack != nullptr );

    if ( stack->size >= stack->capacity - 2 ) StackAllocation ( stack );

    stack->data[stack->size++] = value;

    //PrintStackElements ( stack );

    #ifdef DEBUG
    stack->sum_elm_check += value;
    #endif

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackPop ( Stack_t* stack, STK_ELM_TYPE* value ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    assert ( stack != nullptr );

    *value = stack->data[stack->size - 1];

    stack->data[stack->size - 1] = POISON_NUM;
    stack->size--; 

    #ifdef DEBUG
    stack->sum_elm_check -= *value;
    #endif

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackAllocation ( Stack_t* stack ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    #ifdef DEBUG
    size_t realloc_capacity = stack->capacity*2 + 2;
    #else
    size_t realloc_capacity = stack->capacity*2;
    #endif

    stack->data = (STK_ELM_TYPE*) realloc ( stack->data, ( realloc_capacity ) * sizeof(STK_ELM_TYPE) );

    for ( size_t ind = stack->size; ind < realloc_capacity; ind++ ) stack->data[ind] = POISON_NUM;

    stack->capacity = stack->capacity*2;

    #ifdef DEBUG
    stack->data[stack->capacity + 1] = CANARY_NUM;
    #endif

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackSum ( Stack_t* stack ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;
    STK_ELM_TYPE element_1 = 0;
    STK_ELM_TYPE element_2 = 0;

    status = StackPop ( stack, &element_2 );
    STK_STATUS_CHECK

    status = StackPop ( stack, &element_1 );
    STK_STATUS_CHECK

    StackPush ( stack, element_1 + element_2 );
    STK_STATUS_CHECK

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackSub ( Stack_t* stack ) {

    Stack_Err_t status = STK_SUCCSESFUL;

    STK_ELM_TYPE element_1 = 0;
    STK_ELM_TYPE element_2 = 0;

    status = StackPop ( stack, &element_2 );
    STK_STATUS_CHECK
    status = StackPop ( stack, &element_1 );
    STK_STATUS_CHECK

    StackPush ( stack, element_1 - element_2 );
    STK_STATUS_CHECK

    return status;
}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackMult ( Stack_t* stack ) {

    Stack_Err_t status = STK_SUCCSESFUL;

    STK_ELM_TYPE element_1 = 0;
    STK_ELM_TYPE element_2 = 0;

    status = StackPop ( stack, &element_2 );
    STK_STATUS_CHECK
    status = StackPop ( stack, &element_1 );
    STK_STATUS_CHECK

    StackPush ( stack, element_1 * element_2 );
    STK_STATUS_CHECK

    return status;
}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackDiv ( Stack_t* stack ) {

    Stack_Err_t status = STK_SUCCSESFUL;

    STK_ELM_TYPE element_1 = 0;
    STK_ELM_TYPE element_2 = 0;

    status = StackPop ( stack, &element_2 );
    STK_STATUS_CHECK
    status = StackPop ( stack, &element_1 );
    STK_STATUS_CHECK

    StackPush ( stack, element_1 / element_2 );
    STK_STATUS_CHECK

    return status;
}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackOut ( Stack_t* stack ) {

    Stack_Err_t status = STK_SUCCSESFUL;

    STK_ELM_TYPE value = stack->data[stack->size - 1];

    printf( "CURRENT RESULT: %s%ld%s\n", BLUE, value, RES_COL );

    return status;

}
