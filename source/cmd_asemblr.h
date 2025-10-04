#ifndef __CMD_ASEMBLR__
#define __CMD_ASEMBLR__

#include "error_handler.h"

/*-------------------------------------------------------*/

typedef struct {

    const char* name;
    const int cmd_code;
    const int is_arg;

} Cmd_Instr;

static Cmd_Instr Asmblr_Cmd_Instr[] = {

    {.name = "PUSH",  .cmd_code = 1,  .is_arg = 1 },
    {.name = "POP",   .cmd_code = 2,  .is_arg = 0 },
    {.name = "PUSHR", .cmd_code = 42, .is_arg = 1 },
    {.name = "POPR",  .cmd_code = 33, .is_arg = 1 },
    {.name = "SUM",   .cmd_code = 3,  .is_arg = 0 },
    {.name = "SUB",   .cmd_code = 4,  .is_arg = 0 },
    {.name = "DIV",   .cmd_code = 5,  .is_arg = 0 },
    {.name = "MULT",  .cmd_code = 6,  .is_arg = 0 },
    {.name = "SQRT",  .cmd_code = 7,  .is_arg = 0 },
    {.name = "OUT",   .cmd_code = 8,  .is_arg = 0 },
    {.name = "HLT",   .cmd_code = 9,  .is_arg = 0 },

};

/*-------------------------------------------------------*/

Proc_Err_t CmdAssmblr    ( const char* input_file_name, const char* output_file_name );
Proc_Err_t CmdConvToCode ( int elements, char* command, int* cmd_code );



#endif