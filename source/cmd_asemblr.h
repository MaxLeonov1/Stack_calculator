#ifndef __CMD_ASEMBLR__
#define __CMD_ASEMBLR__

#include <ctype.h>

#include "error_handler.h"
#include "general_instructions.h"

/*-------------------------------------------------------*/

typedef struct {

    size_t max_cmd_instr_num;
    size_t cmd_instr_num;

    Cmd_Instr* instr_general;
    Cmd_Instr* instr_sort_code;
    Cmd_Instr* instr_sort_hash;

    char* fread_buffer;
    STK_ELM_TYPE* cmd_buffer;

    long file_lines_num;
    long cmd_num;
    int lables_num;
    int labels[10];
    int spec_param_num;

    const int proc_reg_num;  
    const int def_cmd_num; 

} Cmd_Assemblr_t;

#define INIT_ASM(name) Cmd_Assemblr_t name = { \
    .max_cmd_instr_num = 96,                   \
    .cmd_instr_num = 0,                        \
    .instr_general = nullptr,                  \
    .instr_sort_code = nullptr,                \
    .instr_sort_hash = nullptr,                \
    .fread_buffer = nullptr,                   \
    .cmd_buffer = nullptr,                     \
    .file_lines_num = 0,                       \
    .cmd_num = 0,                              \
    .lables_num = 10,                          \
    .labels = {-1},                            \
    .spec_param_num = 1,                       \
    .proc_reg_num = 10,                        \
    .def_cmd_num = 32 };

/*-------------------------------------------------------------------------------------------------------------------*/
/*========================================ARGUMENT=HANDLERS=AND=CHECKERS=============================================*/
/*-------------------------------------------------------------------------------------------------------------------*/

typedef struct {

    char* cmd;
    char* arg;
    int cmd_code;
    long arg_code;
    int elements;
    int is_cmd;
    int is_comment;
    int has_arg;
    int cmd_line_num;

} Cmd_Line_t;

#define INIT_CMD_LINE( name ) Cmd_Line_t name = { nullptr, nullptr, 0, 0, 0, 0, 0, 0, 0 };

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

static bool checkLable    ( const char* arg ) { return ( arg[0] == ':' ); }

static bool checkRegister ( const char* arg ) { return ( arg[0] == 'R' && arg[2] == 'X' ); }

static bool checkMemArg   ( const char* arg ) { return ( arg[0] == '[' && arg[4] == ']' &&
                                                         arg[1] == 'R' && arg[3] == 'X' ); }

static bool checkNumber   ( const char* arg ) { return ( isdigit(arg[0]) ||
                                                       ( arg[0] == '-' && isdigit(arg[1]) ) ); }

static bool checkNoArg    ( const char* arg ) { return ( arg[0] == '\0' ); }

static bool checkComment  ( const char* arg ) { return ( arg[0] == '#' ); }

/*-------------------------------------------------------------------------------------------------------------------*/

static Proc_Err_t handleLable ( Cmd_Assemblr_t* assmblr, const char* arg, long* arg_code ) {

    int label_ind = atoi ( arg + sizeof(char) );
    *arg_code = assmblr->labels[label_ind];
    return Proc_Err_t::PRC_SUCCSESFUL;

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
    {checkLable,    handleLable   },

};

/*-------------------------------------------------------------------------------------------------------------------*/
/*=================================================MAIN=FUNCTIONS====================================================*/
/*-------------------------------------------------------------------------------------------------------------------*/

Proc_Err_t RunAssmblr        ( const char* input_file_name, const char* output_file_name );
Proc_Err_t CmdAssmblr        ( const char* input_file_name, const char* output_file_name, Cmd_Assemblr_t* assmblr );
Proc_Err_t CmdConvToCode     ( Cmd_Assemblr_t* assmblr, Cmd_Line_t* line_info );
Proc_Err_t ArgConvToCode     ( Cmd_Assemblr_t* assmblr, Cmd_Line_t* line_info );
Proc_Err_t AsmblrScanFile    ( Cmd_Assemblr_t* assmblr );
Proc_Err_t HandleNonCmdCases ( Cmd_Line_t* line_info, Cmd_Assemblr_t* assmblr );
Proc_Err_t ProcessCmdToken   ( Cmd_Line_t* line_info, Cmd_Assemblr_t* assmblr );
Proc_Err_t ProcessArgToken   ( Cmd_Line_t* line_info, Cmd_Assemblr_t* assmblr );
Proc_Err_t AsmblrPrintFile   ( Cmd_Assemblr_t* assmblr, FILE* stream );

/*-------------------------------------------------------------------------------------------------------------------*/

Proc_Err_t ResolveCmdCode ( char* arg, int* cmd_code );

/*-------------------------------------------------------------------------------------------------------------------*/

#endif