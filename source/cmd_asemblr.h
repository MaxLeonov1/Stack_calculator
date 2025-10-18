#ifndef __CMD_ASEMBLR__
#define __CMD_ASEMBLR__

#include "error_handler.h"

/*-------------------------------------------------------*/

typedef struct {

    STK_ELM_TYPE* cmd_buffer;

    int labels[10];
    int spec_param_num;

    /*PROC_INFO*/

    int proc_reg_num;

} Cmd_Assemblr_t;

#define ASSMBLR( name ) Cmd_Assemblr_t name = { \
.cmd_buffer = nullptr, \
.labels = {-1}, \
.spec_param_num = 1, \
.proc_reg_num = 10, };

/*-------------------------------------------------------------------------------------------------------------------*/
/*=================================================MAIN=FUNCTIONS====================================================*/
/*-------------------------------------------------------------------------------------------------------------------*/

Proc_Err_t CmdAssmblr      ( const char* input_file_name, const char* output_file_name, Cmd_Assemblr_t* assmblr );
Proc_Err_t CmdConvToCode   ( int elements, char* command, int* cmd_code, char* arg );
Proc_Err_t ArgConvToCode   ( Cmd_Assemblr_t* assmblr, int elements, char* arg, long* arg_code );
Proc_Err_t AsmblrScanFile  ( Cmd_Assemblr_t* assmblr, FILE* stream, long* cmd_num );
Proc_Err_t AsmblrPrintFile ( Cmd_Assemblr_t* assmblr, FILE* stream, long cmd_num );

/*-------------------------------------------------------------------------------------------------------------------*/

int IsLabel                ( const char* arg );
int RegisterCmdCodeHandler ( char* arg, int cmd_code );

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

    {.name = "PUSHR", .cmd_code = 33, .is_arg = 1 },
    {.name = "POPR",  .cmd_code = 34, .is_arg = 1 },
    {.name = "PRTR",  .cmd_code = 35, .is_arg = 1 },

};

/*-------------------------------------------------------------------------------------------------------------------*/
/*========================================ARGUMENT=HANDLERS=AND=CHECKERS=============================================*/
/*-------------------------------------------------------------------------------------------------------------------*/

typedef struct {

    bool       (*check)   (const char* arg);
    Proc_Err_t (*handler) (Cmd_Assemblr_t* assmblr, const char* arg, long* arg_code );

} ArgHandler;

/*-------------------------------------------------------------------------------------------------------------------*/

static bool checkLable    ( const char* arg ) { return IsLabel(arg); }
static bool checkRegister ( const char* arg ) { return ( arg[0] == 'R' && arg[2] == 'X' ); }
static bool checkNumber   ( const char* arg ) { return ( isdigit(arg[0]) || ( arg[0] == '-' && isdigit(arg[1]) ) ); }
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

/*-------------------------------------------------------------------------------------------------------------------*/

static ArgHandler Handlers[] = {

    {checkNoArg,    handleNoArg   },
    {checkNumber,   handleNumber  },
    {checkRegister, handleRegister},
    {checkLable,    handleLable   },

};

/*-------------------------------------------------------------------------------------------------------------------*/

#endif