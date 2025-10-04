#ifndef __CMD_PROC__
#define __CMD_PROC__

#include "error_handler.h"
#include "stack_creation.h"



typedef struct {

    int           cur_com_ind = 0;
    STK_ELM_TYPE* cmd_buffer = nullptr;
    STK_ELM_TYPE  reg_buffer[5]  = {0};

} Cmd_Proc;

/*-------------------------------------------------------*/

typedef struct {

    const char* name;
    const int cmd_code;
    const int is_arg;

} Cmd_Instr;

/*-------------------------------------------------------*/

typedef enum { //just for understanding cmd in handler cases

    PUSH  = 1,
    POP   = 2,
    SUM   = 3,
    SUB   = 4,
    DIV   = 5,
    MULT  = 6,
    SQRT  = 7,
    PUSHR = 33,
    POPR  = 34,
    OUT   = 8,
    HLT   = 9,

} InterpretCmds;

Proc_Err_t  CmdConvToCode       ( char* command, int elements, int* cmd_code );
Proc_Err_t  CmdAssmblr          ( const char* input_file_name, const char* output_file_name );
Proc_Err_t  CmdInterpreter      ( const char* input_file_name, Stack_t* stack, Stack_Err_t stack_status );
Stack_Err_t InterpretCmdHandler ( Stack_t* stack, int cmd_code, long argument );

#endif