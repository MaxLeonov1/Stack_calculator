#ifndef __CMD_PROC__
#define __CMD_PROC__

#include "error_handler.h"
#include "stack_creation.h"
#include "general_instructions.h"


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

/*---------------------------------------------------------------------------------------*/

Stack_Err_t CmdHandler      ( Cmd_Proc* processor , int cmd_code, STK_ELM_TYPE argument );
Proc_Err_t  CmdProcessor    ( const char* input_file_name,  Stack_Err_t stk_status );
Proc_Err_t  ScanCmdToBuffer ( FILE* input_file, Cmd_Proc* processor );
Proc_Err_t  ProcessCmds     ( Cmd_Proc* processor, Stack_Err_t* stk_status );

#endif