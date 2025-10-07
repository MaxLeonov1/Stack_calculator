#ifndef __CMD_PROC__
#define __CMD_PROC__

#include "error_handler.h"
#include "stack_creation.h"



typedef struct {

    STK_INIT ( proc_stk )
    size_t stk_def_size = 5;

    int  cur_com_ind = 0;
    long cmd_num = 0;

    STK_ELM_TYPE* cmd_buffer = nullptr;
    STK_ELM_TYPE  reg_buffer[5]  = {0};

} Cmd_Proc;

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
    IN    = 10,
    PAUSE = 11,
    JMP   = 12,
    JB    = 13,

} InterpretCmds;

Stack_Err_t CmdHandler      ( Cmd_Proc* processor , int cmd_code, STK_ELM_TYPE argument );
Proc_Err_t  CmdProcessor    ( const char* input_file_name,  Stack_Err_t stk_status );
Proc_Err_t  ScanCmdToBuffer ( FILE* input_file, Cmd_Proc* processor );
Proc_Err_t  ProcessCmds     ( Cmd_Proc* processor, Stack_Err_t* stk_status );

#endif