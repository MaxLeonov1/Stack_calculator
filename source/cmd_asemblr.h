#ifndef __CMD_ASEMBLR__
#define __CMD_ASEMBLR__

#include <ctype.h>

#include "error_handler.h"

/*-------------------------------------------------------*/

typedef struct {

    char* fread_buffer;
    STK_ELM_TYPE* cmd_buffer;

    long cmd_num;
    int lables_num;
    int labels[10];
    int spec_param_num;

    const int proc_reg_num;  
    const int def_cmd_num; 

} Cmd_Assemblr_t;

#define INIT_ASM(name) Cmd_Assemblr_t name = { \
    .fread_buffer = NULL,                      \
    .cmd_buffer = NULL,                        \
    .cmd_num = 0,                              \
    .lables_num = 10,                          \
    .labels = {-1},                            \
    .spec_param_num = 1,                       \
    .proc_reg_num = 10,                        \
    .def_cmd_num = 32 };

/*-------------------------------------------------------------------------------------------------------------------*/
/*=================================================MAIN=FUNCTIONS====================================================*/
/*-------------------------------------------------------------------------------------------------------------------*/

Proc_Err_t CmdAssmblr      ( const char* input_file_name, const char* output_file_name, Cmd_Assemblr_t* assmblr );
Proc_Err_t CmdConvToCode   ( Cmd_Assemblr_t* assmblr, int elements, char* command, int* cmd_code, char* arg );
Proc_Err_t ArgConvToCode   ( Cmd_Assemblr_t* assmblr, int* cmd_code, char* arg, long* arg_code );
Proc_Err_t AsmblrScanFile  ( Cmd_Assemblr_t* assmblr );
Proc_Err_t AsmblrPrintFile ( Cmd_Assemblr_t* assmblr, FILE* stream, long cmd_num );

/*-------------------------------------------------------------------------------------------------------------------*/

Proc_Err_t ResolveCmdCode ( char* arg, int* cmd_code );

/*-------------------------------------------------------*/
/*========+INSTRUCTIONS=FOR=COMMANDS=HANDLING============*/
/*-------------------------------------------------------*/

typedef struct {

    const char* name;
    const int cmd_code;
    const int is_arg;

} Cmd_Instr;

static Cmd_Instr Asmblr_Cmd_Instr[] = {

    {.name = "PUSH",  .cmd_code = 1,  .is_arg = 1 },
    {.name = "POP",   .cmd_code = 2,  .is_arg = 0 },
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
    {.name = "CALL",  .cmd_code = 20, .is_arg = 1 },
    {.name = "RET",   .cmd_code = 21, .is_arg = 0 },
    {.name = "DRAW",  .cmd_code = 22, .is_arg = 0 },
    {.name = "SIN",   .cmd_code = 23, .is_arg = 0 },
    {.name = "COS",   .cmd_code = 24, .is_arg = 0 },

    {.name = "PUSHR", .cmd_code = 33, .is_arg = 1 },
    {.name = "POPR",  .cmd_code = 34, .is_arg = 1 },
    {.name = "PRTR",  .cmd_code = 35, .is_arg = 1 },

    {.name = "PUSHM", .cmd_code = 65, .is_arg = 1 },
    {.name = "POPM",  .cmd_code = 66, .is_arg = 1 },

};

/*-------------------------------------------------------------------------------------------------------------------*/
/*========================================ARGUMENT=HANDLERS=AND=CHECKERS=============================================*/
/*-------------------------------------------------------------------------------------------------------------------*/

typedef struct {

    char* cmd;
    char* arg;
    int cmd_code;
    int arg_code;
    int elements;
    int is_cmd;
    int cmd_line_num;

} Cmd_Line_t;

#define INIT_CMD_LINE( name ) Cmd_Line_t name = { nullptr, nullptr, 0, 0, 0, 0, 0 };

typedef struct {

    bool       (*check)   (const char* arg);
    Proc_Err_t (*handler) (Cmd_Assemblr_t* assmblr, const char* arg, long* arg_code );

} ArgHandler;

typedef enum {

    GENERAL = 0,
    REGISTER = 1,
    MEMORY = 2,

} Arg_t;

/*-------------------------------------------------------------------------------------------------------------------*/

static bool checkLable    ( const char* arg ) { return arg[0] == ':'; }

static bool checkRegister ( const char* arg ) { return ( arg[0] == 'R' && arg[2] == 'X' ); }

static bool checkMemArg   ( const char* arg ) { return ( arg[0] == '[' && arg[4] == ']' &&
                                                         arg[1] == 'R' && arg[3] == 'X' ); }

static bool checkNumber   ( const char* arg ) { return ( isdigit(arg[0]) ||
                                                       ( arg[0] == '-' && isdigit(arg[1]) ) ); }

static bool checkNoArg    ( const char* arg ) { return ( arg[0] == '\0' ); }

/*-------------------------------------------------------------------------------------------------------------------*/

static Proc_Err_t handleLable ( Cmd_Assemblr_t* assmblr, const char* arg, long* arg_code ) {

    int label_ind = atoi ( (const char*)( arg + sizeof(char) ) );
    *arg_code = assmblr->labels[label_ind];
    return Proc_Err_t::PRC_SUCCSESFUL;

}

static Proc_Err_t handleRegister ( Cmd_Assemblr_t* assmblr, const char* arg, long* arg_code ) {

    if ( ( arg[1] - 'A' >= 0 ) && ( arg[1] - 'A' <= assmblr->proc_reg_num ) ) {

        *arg_code = arg[1] - 'A';
        return Proc_Err_t::PRC_SUCCSESFUL;

    } else return Proc_Err_t::UNDEF_REGISTR_NUM_ERR;

}

static Proc_Err_t handleNumber ( Cmd_Assemblr_t* assmblr, const char* arg, long* arg_code ) {

    *arg_code = atoi ( (const char*)arg );
    return Proc_Err_t::PRC_SUCCSESFUL;

}

static Proc_Err_t handleNoArg ( Cmd_Assemblr_t* assmblr, const char* arg, long* arg_code ) {

    *arg_code = 0;
    return Proc_Err_t::PRC_SUCCSESFUL;

}

static Proc_Err_t handleMemArg ( Cmd_Assemblr_t* assmblr, const char* arg, long* arg_code ) {

    if ( ( arg[2] - 'A' >= 0 ) && ( arg[2] - 'A' <= assmblr->proc_reg_num ) ) {

        *arg_code = arg[2] - 'A';
        return Proc_Err_t::PRC_SUCCSESFUL;

    } else return Proc_Err_t::UNDEF_REGISTR_NUM_ERR;


}

/*-------------------------------------------------------------------------------------------------------------------*/

static ArgHandler Handlers[] = {

    {checkNoArg,    handleNoArg   },
    {checkNumber,   handleNumber  },
    {checkLable,    handleLable   },

};

/*-------------------------------------------------------------------------------------------------------------------*/

#endif