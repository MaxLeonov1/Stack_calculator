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

    {"PUSH",  1,  1},
    {"POP",   2,  0},
    {"PUSHR", 42, 1},
    {"POPR",  32, 0},
    {"SUM",   3,  0},
    {"SUB",   4,  0},
    {"DIV",   5,  0},
    {"MULT",  6,  0},
    {"SQRT",  7,  0},
    {"OUT",   8,  0},
    {"HLT",   9,  0},

};

/*-------------------------------------------------------*/

typedef struct {

    const char* reg_name;
    const int   reg_code;

} Proc_Reg_Def;

static Proc_Reg_Def Asmblr_Reg_Def[] = {

    {"RAX", 1},
    {"RBX", 2},
    {"RCX", 3},
    {"RDX", 4},

};

/*-------------------------------------------------------*/

typedef union {

    long  digit;
    char* str_arg;

} Cmd_Arg;

/*-------------------------------------------------------*/

Proc_Err_t CmdAssmblr    ( const char* input_file_name, const char* output_file_name );
Proc_Err_t CmdConvToCode ( char* command, int elements, int* cmd_code );


#endif