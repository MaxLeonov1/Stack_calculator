#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cmd_asemblr.h"
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

    INIT_FILE_INFO ( file_info, input_file_name )
    FileGetInfo ( input_file, &file_info );
    assmblr->file_lines_num = file_info.line_num;

    assmblr->fread_buffer = (char*) calloc ( (size_t)file_info.byte_num, sizeof(char) );
    fread ( assmblr->fread_buffer, sizeof(char), (size_t)file_info.byte_num + 5, input_file );
    assmblr->cmd_buffer = (STK_ELM_TYPE*) calloc ( 2*file_info.line_num, sizeof(STK_ELM_TYPE) );

    AsmblrScanFile( assmblr );

    // assmblr->cmd_buffer[0] = cmd_num;

    // status = AsmblrPrintFile ( assmblr, output_file, cmd_num );

    free(assmblr->cmd_buffer);
    fclose(input_file);
    fclose(output_file);

    return status;

} 

/*-----------------------------------------------------------------------------------------------*/

// Proc_Err_t AsmblrScanFile ( Cmd_Assemblr_t* assmblr, FILE* stream, long* cmd_num ) { //TODO переделать под онегина

//     Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

//     char* cmd_line = (char*) calloc ( max_cmd_line_size, sizeof(char) );

//     int cmd_ind = assmblr->spec_param_num;

//     while ( fgets ( cmd_line, max_cmd_line_size, stream ) ) {

//         char cmd[max_cmd_size] = {0};
//         char arg[max_cmd_size] = {0};

//         int  cmd_code = 0;
//         long arg_code = 0;
//         int  elements = 0;
        
//         elements = sscanf ( cmd_line, "%s %s", cmd, arg );

//         if ( elements == -1 ) {
//             (*cmd_num)--;
//             continue;
//         }

//         if ( IsLabel(cmd) ) {

//             int label_ind = atoi ( (const char*)( cmd + sizeof(char) ) );
//             assmblr->labels[label_ind] = (cmd_ind - assmblr->spec_param_num)/2;

//             (*cmd_num)--;
//             continue;

//         }

//         status = CmdConvToCode ( assmblr, elements, cmd, &cmd_code, arg );
//         PROC_STATUS_CHECK
//         status = ArgConvToCode ( assmblr, &cmd_code, arg, &arg_code );
//         PROC_STATUS_CHECK

//         assmblr->cmd_buffer[cmd_ind] = cmd_code;
//         assmblr->cmd_buffer[cmd_ind + 1] = arg_code;

//         cmd_ind += 2;

//     }

//     free(cmd_line);

// }

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t AsmblrScanFile ( Cmd_Assemblr_t* assmblr ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    INIT_CMD_LINE ( line_info )

    char* line = strtok ( assmblr->fread_buffer, "\n" );
    line_info.cmd = (char*) calloc ( max_cmd_size, sizeof(char) );
    line_info.arg = (char*) calloc ( max_cmd_size, sizeof(char) );
    
    while (line != NULL && line_info.cmd_line_num < assmblr->file_lines_num) {
        
        if (line[0] != '\0') {
            
        line_info.elements = sscanf ( (const char*)line, "%s %s", line_info.cmd, line_info.arg );

        status = HandleNonCmdCases ( &line_info, assmblr );
        PROC_STATUS_CHECK

            if ( (line_info.is_cmd == 1) && (line_info.is_comment == 0) ) {

                status = CmdConvToCode ( assmblr, &line_info );
                PROC_STATUS_CHECK

                status = ArgConvToCode ( assmblr, &line_info );
                PROC_STATUS_CHECK

            }

        }
        
        line = strtok(NULL, "\n");
    }

    assmblr->cmd_num = line_info.cmd_line_num;

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t HandleNonCmdCases ( Cmd_Line_t* line_info, Cmd_Assemblr_t* assmblr ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    status = ProcessCmdToken ( line_info, assmblr );
    PROC_STATUS_CHECK

    status = ProcessArgToken ( line_info, assmblr );
    PROC_STATUS_CHECK

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t ProcessCmdToken ( Cmd_Line_t* line_info, Cmd_Assemblr_t* assmblr ) {

    if ( checkLable ( line_info->cmd ) ) {

        line_info->is_cmd = 0;

        int label_ind = atoi ( (const char*)( line_info->cmd + sizeof(char) ) );
        if ( ( 0 < label_ind ) && ( label_ind >= assmblr->lables_num ) ) 
            return Proc_Err_t::INCOR_LABLE_NUM;
        assmblr->labels[label_ind] = assmblr->cmd_num;
        return Proc_Err_t::PRC_SUCCSESFUL;

    }

    if ( checkComment ( line_info->cmd ) ) {

        line_info->is_cmd = 0;

        line_info->is_comment = 1;
        return Proc_Err_t::PRC_SUCCSESFUL;

    }

    line_info->is_cmd = 1;
    return Proc_Err_t::PRC_SUCCSESFUL;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t ProcessArgToken ( Cmd_Line_t* line_info, Cmd_Assemblr_t* assmblr ) {

    if ( checkComment ( line_info->arg ) ) {

        line_info->arg = nullptr;
        line_info->elements--;
        return Proc_Err_t::PRC_SUCCSESFUL;

    }

    return Proc_Err_t::PRC_SUCCSESFUL;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t AsmblrPrintFile ( Cmd_Assemblr_t* assmblr, FILE* stream, long cmd_num ) {

    int ind = 0;

    while ( ind < 2*( cmd_num ) + assmblr->spec_param_num ) {

        fprintf ( stream, "%ld ", (long)assmblr->cmd_buffer[ind] );
        ind++;

    }

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t CmdConvToCode ( Cmd_Assemblr_t* assmblr, Cmd_Line_t* line_info ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    for ( size_t ind = 0; ind < sizeof(Asmblr_Cmd_Instr) / sizeof(Asmblr_Cmd_Instr[0]); ind++ ) {

        if ( strcmp ( line_info->cmd, Asmblr_Cmd_Instr[ind].name ) == 0 ) { //TODO хэш с bin поиском 

            line_info->cmd_code = Asmblr_Cmd_Instr[ind].cmd_code;
            //printf ( "cmd: %s\n", Asmblr_Cmd_Instr[ind].name );
            status = ResolveCmdCode ( line_info->arg, &line_info->cmd_code );
            PROC_STATUS_CHECK
            //printf ("code: %d\n", *cmd_code);

            if ( line_info->cmd_code <= assmblr->def_cmd_num )
                if ( (  Asmblr_Cmd_Instr[ind].is_arg && line_info->elements < 2 ) ||
                     ( !Asmblr_Cmd_Instr[ind].is_arg && line_info->elements > 1 ))
                    return Proc_Err_t::INCOR_ARG_NUM_ERR;

            return Proc_Err_t::PRC_SUCCSESFUL;

        }

    }

    return Proc_Err_t::UNDEF_COMAND_ERR;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t ArgConvToCode ( Cmd_Assemblr_t* assmblr, Cmd_Line_t* cmd_line ) {

    char arg_t = (char)( cmd_line->cmd_code >> 5 );

    switch (arg_t) {

        case Arg_t::GENERAL:

            for ( size_t i = 0; i < sizeof(Handlers) / sizeof(Handlers[0]); i++ ) {

                if ( Handlers[i].check(cmd_line->arg) ) {

                    return Handlers[i].handler(assmblr, cmd_line->arg, &cmd_line->arg_code);

                }

            }

            return Proc_Err_t::INCOR_ARG_ERR;
        
        case Arg_t::REGISTER:

            cmd_line->arg_code = cmd_line->arg[1] - 'A';
            REG_EXISTANCE_CHECK ( cmd_line->arg_code )

        case Arg_t::MEMORY:

            cmd_line->arg_code = cmd_line->arg[2] - 'A';
            REG_EXISTANCE_CHECK ( cmd_line->arg_code )

    }

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t ResolveCmdCode ( char* arg, int* cmd_code ) {

    if ( arg[0] == 'R' && arg[2] == 'X' ) {

        if ( *cmd_code < 32 ) *cmd_code += 32;
        else if ( *cmd_code > 64 ) return Proc_Err_t::INCOR_ARG_ERR;
    
    }

    if ( arg[0] == '[' && arg[4] == ']' &&
         arg[1] == 'R' && arg[3] == 'X' ) {

        if ( *cmd_code < 32 ) *cmd_code += 64;
        else if ( *cmd_code < 64 ) return Proc_Err_t::INCOR_ARG_ERR;

    }

    return Proc_Err_t::PRC_SUCCSESFUL;
    
}

/*-----------------------------------------------------------------------------------------------*/


