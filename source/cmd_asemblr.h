#ifndef __CMD_ASEMBLR__
#define __CMD_ASEMBLR__

#include "error_handler.h"

/*-------------------------------------------------------*/

typedef struct {

    const char* name;
    const int cmd_code;
    const int is_arg;

} Cmd_Instr;

typedef struct {

    int labels[10];

} Cmd_Assemblr_t;

#define ASSMBLR( name ) Cmd_Assemblr_t name = { .labels = {-1} };

static Cmd_Instr Asmblr_Cmd_Instr[] = {

    {.name = "PUSH",  .cmd_code = 1,  .is_arg = 1 },
    {.name = "POP",   .cmd_code = 2,  .is_arg = 0 },
    {.name = "PUSHR", .cmd_code = 33, .is_arg = 1 },
    {.name = "POPR",  .cmd_code = 34, .is_arg = 1 },
    {.name = "SUM",   .cmd_code = 3,  .is_arg = 0 },
    {.name = "SUB",   .cmd_code = 4,  .is_arg = 0 },
    {.name = "DIV",   .cmd_code = 5,  .is_arg = 0 },
    {.name = "MULT",  .cmd_code = 6,  .is_arg = 0 },
    {.name = "SQRT",  .cmd_code = 7,  .is_arg = 0 },
    {.name = "OUT",   .cmd_code = 8,  .is_arg = 0 },
    {.name = "HLT",   .cmd_code = 9,  .is_arg = 0 },
    {.name = "IN",    .cmd_code = 10, .is_arg = 0 },
    {.name = "PAUSE", .cmd_code = 11, .is_arg = 0 },
    {.name = "JMP",   .cmd_code = 12, .is_arg = 1 },
    {.name = "JB",    .cmd_code = 13, .is_arg = 1 },
    {.name = "JBE",   .cmd_code = 14, .is_arg = 1 },
    {.name = "JA",    .cmd_code = 15, .is_arg = 1 },
    {.name = "JAE",   .cmd_code = 16, .is_arg = 1 },
    {.name = "JE" ,   .cmd_code = 17, .is_arg = 1 },
    {.name = "JNE",   .cmd_code = 18, .is_arg = 1 },
    {.name = "PRTS",  .cmd_code = 19, .is_arg = 0 },

};

/*-------------------------------------------------------*/

Proc_Err_t CmdAssmblr      ( const char* input_file_name, const char* output_file_name, Cmd_Assemblr_t* assmblr );
Proc_Err_t CmdConvToCode   ( int elements, char* command, int* cmd_code );
Proc_Err_t ArgConvToCode   ( Cmd_Assemblr_t* assmblr, int elements, char* arg, long* arg_code );
Proc_Err_t AsmblrScanFile  ( Cmd_Assemblr_t* assmblr, STK_ELM_TYPE* cmd_buffer, FILE* stream, long* cmd_num );
Proc_Err_t AsmblrPrintFile ( STK_ELM_TYPE* cmd_buffer, FILE* stream, long cmd_num );

int IsLabel ( const char* arg );



#endif