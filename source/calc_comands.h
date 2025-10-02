#ifndef __CALC_CMD__
#define __CALC_CMD__


#include "stack_creation.h"
#include "error_handler.h"

Stack_Err_t StackPush       ( Stack_t* stack, STK_ELM_TYPE value );
Stack_Err_t StackPop        ( Stack_t* stack, STK_ELM_TYPE* value );
Stack_Err_t StackAllocation ( Stack_t* stack );
Stack_Err_t StackSum        ( Stack_t* stack );
Stack_Err_t StackSub        ( Stack_t* stack );
Stack_Err_t StackMult       ( Stack_t* stack );
Stack_Err_t StackDiv        ( Stack_t* stack );
Stack_Err_t StackOut        ( Stack_t* stack );
Stack_Err_t StackSqrt       ( Stack_t* stack );

#endif