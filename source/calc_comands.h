#ifndef __CALC_CMD__
#define __CALC_CMD__


#include "stack_creation.h"
#include "error_handler.h"


typedef enum {

    BELOW = 0,
    BELOW_AND_EQUAL = 1,
    GREATER = 3,
    GREATER_AND_EQUAL = 4,
    EQUAL = 5,
    NOT_EQUAL = 6,

} Cmd_Jump_t;


Stack_Err_t StackPush       ( Stack_t* stack, STK_ELM_TYPE value );
Stack_Err_t StackPop        ( Stack_t* stack, STK_ELM_TYPE* value );
Stack_Err_t StackAllocation ( Stack_t* stack );
Stack_Err_t StackSum        ( Stack_t* stack );
Stack_Err_t StackSub        ( Stack_t* stack );
Stack_Err_t StackMult       ( Stack_t* stack );
Stack_Err_t StackDiv        ( Stack_t* stack );
Stack_Err_t StackOut        ( Stack_t* stack );
Stack_Err_t StackSqrt       ( Stack_t* stack );
Stack_Err_t StackIn         ( Stack_t* stack );
Stack_Err_t RegistrPop      ( Cmd_Proc* processor, int reg_num );
Stack_Err_t RegistrPush     ( Cmd_Proc* processor, int reg_num );
Stack_Err_t PauseProc       ( void );
Stack_Err_t PrintRegValue   ( Cmd_Proc* processor, STK_ELM_TYPE reg_num );
Stack_Err_t ReturnToCall    ( Cmd_Proc* processor );
Stack_Err_t CallCmd         ( Cmd_Proc* processor, STK_ELM_TYPE jmp_ind );
Stack_Err_t JumpToCmd       ( Cmd_Proc* processor, STK_ELM_TYPE cmd_ind );
Stack_Err_t JumpIf          ( Cmd_Proc* processor, STK_ELM_TYPE cmd_ind, Cmd_Jump_t type );

#endif