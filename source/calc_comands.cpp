#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "error_handler.h"
#include "stack_creation.h"
#include "cmd_processor.h"
#include "calc_comands.h"
#include "colors.h"

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackPush ( Stack_t* stack, STK_ELM_TYPE value ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    assert ( stack != nullptr );

    if ( stack->size >= stack->capacity - 2 ) status = StackAllocation ( stack );
    STK_STATUS_CHECK

    stack->data[stack->size++] = value;

    //PrintStackElements ( stack );

    #ifdef DEBUG
    stack->sum_elm_check += value;
    #endif

    status = StackErrorHandler ( stack, true );
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

    status = StackErrorHandler ( stack, true );
    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t RegistrPush ( Cmd_Proc* proc, int reg_num ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    status = StackPush ( &proc->proc_stk, proc->reg_buffer[reg_num] );
    STK_STATUS_CHECK
    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t RegistrPop ( Cmd_Proc* proc, int reg_num ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    STK_ELM_TYPE element = 0;
    
    status = StackPop ( &proc->proc_stk , &element );
    STK_STATUS_CHECK

    proc->reg_buffer[reg_num] = element;

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackIn ( Stack_t* stack ) {

    Stack_Err_t  status = Stack_Err_t::STK_SUCCSESFUL;
    STK_ELM_TYPE value  = 0;

    printf ( "Inter Push Value: " );
    scanf ( "%ld", &value );

    status = StackPush ( stack, value );

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
    status = StackErrorHandler ( stack, true );
    STK_STATUS_CHECK

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

    //printf("%ld %ld\n", element_1, element_2);

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

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t StackSqrt ( Stack_t* stack ) {

    Stack_Err_t status = STK_SUCCSESFUL;

    STK_ELM_TYPE element_1 = 0;

    status = StackPop ( stack, &element_1 );
    STK_STATUS_CHECK

    StackPush ( stack, (STK_ELM_TYPE)sqrt( element_1 ) );
    STK_STATUS_CHECK

    return status;
}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t JumpToCmd ( Cmd_Proc* processor, STK_ELM_TYPE cmd_ind ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    processor->cur_com_ind = cmd_ind;

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t PauseProc ( void ) {

    getchar();

}

/*-----------------------------------------------------------------------------------------------*/

Stack_Err_t JumpIf ( Cmd_Proc* processor, STK_ELM_TYPE cmd_ind ) {

    Stack_Err_t status = Stack_Err_t::STK_SUCCSESFUL;

    STK_ELM_TYPE element_1 = 0;
    STK_ELM_TYPE element_2 = 0;

    status = StackPop ( &processor->proc_stk, &element_2 );
    STK_STATUS_CHECK
    status = StackPop ( &processor->proc_stk, &element_1 );
    STK_STATUS_CHECK

    if ( element_1 < element_2 ) status = JumpToCmd ( processor, cmd_ind );
    STK_STATUS_CHECK

    return status;

}