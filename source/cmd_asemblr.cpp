#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cmd_asemblr.h"
#include "support_functions.h"
#include "error_handler.h"



const size_t max_cmd_line_size = 100;
const size_t max_cmd_size = 20;

const int proc_reg_num = 5;
const int spec_param_num = 1;

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t CmdAssmblr ( const char* input_file_name, const char* output_file_name, Cmd_Assemblr_t* assmblr ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    FILE* input_file = fopen ( input_file_name, "r" );
    FILE* output_file = fopen ( output_file_name, "wb" );

    if ( input_file == nullptr || output_file == nullptr ) 
        return Proc_Err_t::FILE_OPEN_ERR;

    long cmd_num = FileLineCount(input_file_name);
    if ( cmd_num == 0 ) return Proc_Err_t::FILE_OPEN_ERR;

    STK_ELM_TYPE* cmd_buffer = (STK_ELM_TYPE*) calloc ( 2*cmd_num + 5, sizeof(STK_ELM_TYPE) );

    status = AsmblrScanFile ( assmblr, cmd_buffer, input_file, &cmd_num );

    cmd_buffer[0] = cmd_num;

    status = AsmblrPrintFile ( cmd_buffer, output_file, cmd_num );

    free(cmd_buffer);
    fclose(input_file);
    fclose(output_file);

    return status;

} 

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t AsmblrScanFile ( Cmd_Assemblr_t* assmblr, STK_ELM_TYPE* cmd_buffer, FILE* stream, long* cmd_num ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    char* cmd_line = (char*) calloc ( max_cmd_line_size, sizeof(char) );

    int cmd_ind = spec_param_num;

    while ( fgets ( cmd_line, max_cmd_line_size, stream ) ) {

        char command[max_cmd_size] = {0};
        char argument[max_cmd_size] = {0};

        int  cmd_code = 0;
        long arg_code = 0;
        int  elements = 0;

        //printf ( "%s", cmd_line );
        
        elements = sscanf ( cmd_line, "%s %s", command, argument );

        //printf ( "%d\n", elements );

        if ( elements == -1 ) {
            (*cmd_num)--;
            continue;
        }

        if ( IsLabel(command) ) {

            int label_ind = atoi ( (const char*)( command + sizeof(char) ) );
            assmblr->labels[label_ind] = (cmd_ind - spec_param_num)/2;

            (*cmd_num)--;
            continue;

        }

        status = CmdConvToCode ( elements, command, &cmd_code );
        PROC_STATUS_CHECK
        status = ArgConvToCode ( assmblr, elements, argument, &arg_code );
        PROC_STATUS_CHECK

        cmd_buffer[cmd_ind]     = cmd_code;
        cmd_buffer[cmd_ind + 1] = arg_code;

        cmd_ind += 2;

    }

    free(cmd_line);

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t AsmblrPrintFile ( STK_ELM_TYPE* cmd_buffer, FILE* stream, long cmd_num ) {

    int ind = 0;

    while ( ind < 2*( cmd_num ) + spec_param_num ) {

        fprintf ( stream, "%ld ", cmd_buffer[ind] );
        ind++;

    }

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t CmdConvToCode ( int elements, char* command, int* cmd_code ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    for ( size_t ind = 0; ind < sizeof(Asmblr_Cmd_Instr) / sizeof(Asmblr_Cmd_Instr[0]); ind++ ) {

        if ( strcmp ( command, Asmblr_Cmd_Instr[ind].name) == 0 ) { 

            *cmd_code = Asmblr_Cmd_Instr[ind].cmd_code;

            //printf("%d %s\n", *cmd_code, Asmblr_Cmd_Instr[ind].name );
            
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

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    if ( IsLabel(arg) ) {

        int label_ind = atoi ( (const char*)( arg + sizeof(char) ) );
        *arg_code = assmblr->labels[label_ind];
        return status;
    
    }

    if ( isdigit(arg[0]) || arg[0] == '-' ) {

        *arg_code = atoi ( (const char*)arg );
        return status;
        //printf("aboba\n");
    
    }

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

int SelectCorrectCmd ( char* cmd, const char* cmd_compare ) {


    
}