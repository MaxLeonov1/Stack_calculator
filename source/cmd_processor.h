#ifndef __ASSMBLR__
#define __ASSMBLR__

#include "error_handler.h"
#include "stack_creation.h"

typedef struct {

    const char* name;
    const int cmd_code;
    const int is_arg;

} Cmd_Instr;

static Cmd_Instr Asmblr_Cmd_Instr[] = {

    {"PUSH", 1, 1},
    {"POP",  2, 0},
    {"SUM",  3, 0},
    {"SUB",  4, 0},
    {"DIV",  5, 0},
    {"MULT", 6, 0},
    {"OUT",  7, 0},
    {"HLT",  8, 0},

};

typedef enum { //just for understanding cmd in handler cases

    PUSH = 1,
    POP  = 2,
    SUM  = 3,
    SUB  = 4,
    DIV  = 5,
    MULT = 6,
    OUT  = 7,
    HLT  = 8,

} InterpretCmds;

Proc_Err_t  CmdConvToCode       ( char* command, int elements, int* cmd_code );
Proc_Err_t  CmdAssmblr          ( const char* input_file_name, const char* output_file_name );
Proc_Err_t  CmdInterpreter      ( const char* input_file_name, Stack_t* stack, Stack_Err_t stack_status );
Stack_Err_t InterpretCmdHandler ( Stack_t* stack, int cmd_code, long argument );

#endif