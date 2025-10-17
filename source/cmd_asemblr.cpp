#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cmd_asemblr.h"
#include "cmd_processor.h"
#include "support_functions.h"
#include "error_handler.h"

const size_t max_cmd_line_size = 100;
const size_t max_cmd_size = 20;

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t CmdAssmblr ( const char* input_file_name, const char* output_file_name, Cmd_Assemblr_t* assmblr ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    FILE* input_file = fopen ( input_file_name, "r" );
    FILE* output_file = fopen ( output_file_name, "wb" );

    if ( input_file == nullptr || output_file == nullptr ) 
        return Proc_Err_t::FILE_OPEN_ERR;

    long cmd_num = FileLineCount(input_file_name);
    if ( cmd_num == 0 ) return Proc_Err_t::FILE_OPEN_ERR;

    assmblr->cmd_buffer = (STK_ELM_TYPE*) calloc ( 2*cmd_num + 5, sizeof(STK_ELM_TYPE) );

    status = AsmblrScanFile ( assmblr, input_file, &cmd_num );

    assmblr->cmd_buffer[0] = cmd_num;

    status = AsmblrPrintFile ( assmblr, output_file, cmd_num );

    free(assmblr->cmd_buffer);
    fclose(input_file);
    fclose(output_file);

    return status;

} 

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t AsmblrScanFile ( Cmd_Assemblr_t* assmblr, FILE* stream, long* cmd_num ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    char* cmd_line = (char*) calloc ( max_cmd_line_size, sizeof(char) );

    int cmd_ind = assmblr->spec_param_num;

    while ( fgets ( cmd_line, max_cmd_line_size, stream ) ) {

        char cmd[max_cmd_size] = {0};
        char arg[max_cmd_size] = {0};

        int  cmd_code = 0;
        long arg_code = 0;
        int  elements = 0;
        
        elements = sscanf ( cmd_line, "%s %s", cmd, arg );

        if ( elements == -1 ) {
            (*cmd_num)--;
            continue;
        }

        if ( IsLabel(cmd) ) {

            int label_ind = atoi ( (const char*)( cmd + sizeof(char) ) );
            assmblr->labels[label_ind] = (cmd_ind - assmblr->spec_param_num)/2;

            (*cmd_num)--;
            continue;

        }

        status = CmdConvToCode ( elements, cmd, &cmd_code, arg );
        PROC_STATUS_CHECK
        status = ArgConvToCode ( assmblr, elements, arg, &arg_code );
        PROC_STATUS_CHECK

        assmblr->cmd_buffer[cmd_ind] = cmd_code;
        assmblr->cmd_buffer[cmd_ind + 1] = arg_code;

        cmd_ind += 2;

    }

    free(cmd_line);

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t AsmblrPrintFile ( Cmd_Assemblr_t* assmblr, FILE* stream, long cmd_num ) {

    int ind = 0;

    while ( ind < 2*( cmd_num ) + assmblr->spec_param_num ) {

        fprintf ( stream, "%ld ", assmblr->cmd_buffer[ind] );
        ind++;

    }

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t CmdConvToCode ( int elements, char* cmd, int* cmd_code, char* arg ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    for ( size_t ind = 0; ind < sizeof(Asmblr_Cmd_Instr) / sizeof(Asmblr_Cmd_Instr[0]); ind++ ) {

        if ( strcmp ( cmd, Asmblr_Cmd_Instr[ind].name) == 0 ) { 

            // printf ( "cmd: %s\n", Asmblr_Cmd_Instr[ind].name );
            *cmd_code = RegisterCmdCodeHandler ( arg, Asmblr_Cmd_Instr[ind].cmd_code );
            // printf ("code: %d\n", *cmd_code);

            if ( *cmd_code <= 32 )
                if ( (  Asmblr_Cmd_Instr[ind].is_arg && elements < 2 ) ||
                     ( !Asmblr_Cmd_Instr[ind].is_arg && elements > 1 ))
                    return Proc_Err_t::INCOR_ARG_NUM_ERR;

            return Proc_Err_t::PRC_SUCCSESFUL;

        }

    }

    return Proc_Err_t::UNDEF_COMAND_ERR;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t ArgConvToCode ( Cmd_Assemblr_t* assmblr, int elements, char* arg, long* arg_code ) {

    if ( IsLabel(arg) ) {

        int label_ind = atoi ( (const char*)( arg + sizeof(char) ) );
        *arg_code = assmblr->labels[label_ind];
        return Proc_Err_t::PRC_SUCCSESFUL;
    
    }

    if ( isdigit(arg[0]) || arg[0] == '-' ) {

        *arg_code = atoi ( (const char*)arg );
        return Proc_Err_t::PRC_SUCCSESFUL;
        //printf("aboba\n");
    
    }

    if ( arg[0] == 'R' && arg[2] == 'X' ) {

        if ( ( arg[1] - 'A' >= 0 ) && ( arg[1] - 'A' <= assmblr->proc_reg_num ) ) {

            *arg_code = arg[1] - 'A';
            return Proc_Err_t::PRC_SUCCSESFUL;

        } else return Proc_Err_t::UNDEF_REGISTR_NUM_ERR;

    }

    if ( arg[0] == '\0' ) {

        *arg_code = 0;
        return Proc_Err_t::PRC_SUCCSESFUL;

    }

    return Proc_Err_t::INCOR_ARG_ERR;

}

/*-----------------------------------------------------------------------------------------------*/

int IsLabel ( const char* arg ) {

    if ( arg[0] == ':' ) {

        return 1;

    } else {

        return 0;

    }

}

/*-----------------------------------------------------------------------------------------------*/

int RegisterCmdCodeHandler ( char* arg, int cmd_code ) {

    //printf ("arg: %s\n", arg);

    if ( arg[0] == 'R' ) {

        //printf ("%d\n", cmd_code);

        if ( cmd_code > 32 ) return cmd_code;

        else return cmd_code + 32;
    
    } else return cmd_code;
    
}

/*-----------------------------------------------------------------------------------------------*/

// Proc_Err_t NonCmdCasesCheck ( int elements, long* cmd_num ) {

//     if ( elements == -1 ) {
//         (*cmd_num)--;
//         continue;
//     }

//     if ( IsLabel(cmd) ) {

//         int label_ind = atoi ( (const char*)( cmd + sizeof(char) ) );
//         assmblr->labels[label_ind] = (cmd_ind - assmblr->spec_param_num)/2;

//         (*cmd_num)--;
//         continue;

//     }

// }