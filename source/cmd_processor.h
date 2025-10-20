#ifndef __CMD_PROC__
#define __CMD_PROC__

#include "error_handler.h"
#include "stack_creation.h"



typedef struct {

    STK_INIT ( proc_stk )
    STK_INIT ( call_stk )
    size_t stk_def_size = 5;

    int  cur_com_ind = 0;
    long cmd_num = 0;
    long ram_size = 2400;

    STK_ELM_TYPE* cmd_buffer = nullptr;
    STK_ELM_TYPE  reg_buffer[10] = {0};
    STK_ELM_TYPE* ram = (STK_ELM_TYPE*) calloc ( ram_size, sizeof(STK_ELM_TYPE) );

} Cmd_Proc;

/*-------------------------------------------------------*/

typedef enum { //just for understanding cmd in handler cases

    NULL_CMD = 0,
    PUSH  = 1,
    POP   = 2,
    SUM   = 3,
    SUB   = 4,
    DIV   = 5,
    MULT  = 6,
    SQRT  = 7,
    OUT   = 8,
    HLT   = 9,
    IN    = 10,
    PAUSE = 11,
    JMP   = 12,
    JB    = 13,
    JBE   = 14,
    JA    = 15,
    JAE   = 16,
    JE    = 17,
    JNE   = 18,
    PRTS  = 19,
    CALL  = 20,
    RET   = 21,
    DRAW  = 22,
    SIN   = 23,
    COS   = 24,
    PUSHR = 33,
    POPR  = 34,
    PRTR  = 35,
    PUSHM = 65,
    POPM  = 66,

} InterpretCmds;

Stack_Err_t CmdHandler      ( Cmd_Proc* processor , int cmd_code, STK_ELM_TYPE argument );
Proc_Err_t  CmdProcessor    ( const char* input_file_name,  Stack_Err_t stk_status );
Proc_Err_t  ScanCmdToBuffer ( FILE* input_file, Cmd_Proc* processor );
Proc_Err_t  ProcessCmds     ( Cmd_Proc* processor, Stack_Err_t* stk_status );

#endif