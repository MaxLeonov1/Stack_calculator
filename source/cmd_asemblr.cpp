#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cmd_asemblr.h"
#include "error_handler.h"



const size_t max_cmd_line_size = 100;
const size_t max_cmd_size = 20;


Proc_Err_t CmdAssmblr ( const char* input_file_name, const char* output_file_name ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    FILE* input_file = fopen ( input_file_name, "r" );
    FILE* output_file = fopen ( output_file_name, "wb" );

    if ( input_file == nullptr || output_file == nullptr ) 
        return Proc_Err_t::FILE_OPEN_ERR;

    char* cmd_line = (char*) calloc ( max_cmd_line_size, sizeof(char) );

    while ( fgets ( cmd_line, max_cmd_line_size, input_file ) ) {

        char* comand = (char*) calloc ( max_cmd_size, sizeof(char) );
        int cmd_code = 0;
        Cmd_Arg arg  = {0};
        int elments  = 0;
        
        elments += sscanf ( cmd_line, "%s %ld", comand, &arg.digit);
        // elments += sscanf ( cmd_line, "%ld", &arg.digit);

        // printf("%s %ld\n", comand, arg.digit);
        // printf("%d\n", elments);

        if ( elments == 0 ) continue;

        status = CmdConvToCode ( comand, elments, &cmd_code );

        if ( status != Proc_Err_t::PRC_SUCCSESFUL )
            return status;

        fprintf ( output_file, "%d %ld\n", cmd_code, arg.digit );

        free(comand);

    }

    free(cmd_line);
    fclose(input_file);
    fclose(output_file);

    return status;

} 



Proc_Err_t CmdConvToCode ( char* command, int elements, int* cmd_code ) {

    for ( size_t ind = 0; ind < sizeof(Asmblr_Cmd_Instr) / sizeof(Asmblr_Cmd_Instr[0]); ind++ ) {

        if ( strcmp ( command, Asmblr_Cmd_Instr[ind].name) == 0) { 

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
