#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cmd_asemblr.h"
#include "error_handler.h"



const size_t max_cmd_line_size = 100;
const size_t max_cmd_size = 20;

const int proc_reg_num = 5;


Proc_Err_t CmdAssmblr ( const char* input_file_name, const char* output_file_name ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    FILE* input_file = fopen ( input_file_name, "r" );
    FILE* output_file = fopen ( output_file_name, "wb" );

    if ( input_file == nullptr || output_file == nullptr ) 
        return Proc_Err_t::FILE_OPEN_ERR;

    char* cmd_line = (char*) calloc ( max_cmd_line_size, sizeof(char) );

    while ( fgets ( cmd_line, max_cmd_line_size, input_file ) ) {

        char comand[max_cmd_size] = {0};

        int  cmd_code = 0;
        long arg      = 0;
        int  elements = 0;
        
        elements = sscanf ( cmd_line, "%s %ld", comand, &arg );

        if ( elements == 0 ) continue;

        status = CmdConvToCode ( elements, comand, &cmd_code );
        PROC_STATUS_CHECK

        fprintf ( output_file, "%d %ld ", cmd_code, arg );

    }

    free(cmd_line);
    fclose(input_file);
    fclose(output_file);

    return status;

} 



Proc_Err_t CmdConvToCode ( int elements, char* command, int* cmd_code ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    for ( size_t ind = 0; ind < sizeof(Asmblr_Cmd_Instr) / sizeof(Asmblr_Cmd_Instr[0]); ind++ ) {

        if ( strcmp ( command, Asmblr_Cmd_Instr[ind].name) == 0) { 

            *cmd_code = Asmblr_Cmd_Instr[ind].cmd_code;

            PROC_STATUS_CHECK

            //printf("%d %s\n", *cmd_code, Asmblr_Cmd_Instr[ind].name );
            
            if ( (  Asmblr_Cmd_Instr[ind].is_arg && elements < 2 ) ||
                 ( !Asmblr_Cmd_Instr[ind].is_arg && elements > 1 ))
                return Proc_Err_t::INCOR_ARG_NUM_ERR;

            return Proc_Err_t::PRC_SUCCSESFUL;

        }

    }

    return Proc_Err_t::UNDEF_COMAND_ERR;

}