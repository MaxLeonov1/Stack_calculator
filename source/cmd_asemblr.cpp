#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cmd_asemblr.h"
#include "support_functions.h"

const size_t max_cmd_num = 96;
const size_t max_cmd_size = 20;

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t RunAssmblr ( const char* input_file_name, const char* output_file_name ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;
    INIT_ASM ( assmblr )

    AsmCtor ( &assmblr );

    CopyAndHashCmdInstr ( Cmd_Interpret_Instr, &assmblr );
    SortByCodeInstructionsTable ( &assmblr );
    SortByHashInstructionsTable ( &assmblr );

    status = CmdAssmblr ( input_file_name, output_file_name, &assmblr );
    PROC_STATUS_CHECK

    AsmDtor ( &assmblr );

    return status;

}

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
    assmblr->cmd_buffer = (long*) calloc ( 2*file_info.line_num, sizeof(STK_ELM_TYPE) );

    assmblr->cmd_num = assmblr->spec_param_num;

    status = AsmblrScanFile( assmblr );
    PROC_STATUS_CHECK
    // status = AsmblrScanFile( assmblr );
    // PROC_STATUS_CHECK

    InsertSpecParamToBuffer ( assmblr );

    // for ( int i = 0; i < assmblr->cmd_num; i++ ) 
    //     printf("%d\n", assmblr->cmd_buffer[i] );

    status = AsmblrPrintFile ( assmblr, output_file );
    PROC_STATUS_CHECK

    free(assmblr->fread_buffer);
    free(assmblr->cmd_buffer);
    fclose(input_file);
    fclose(output_file);

    return status;

} 

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t AsmblrScanFile ( Cmd_Assemblr_t* assmblr ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    INIT_CMD_LINE ( line_info )

    char* line = strtok ( assmblr->fread_buffer, "\n" );
    line_info.cmd = (char*) calloc ( max_cmd_size, sizeof(char) );
    line_info.arg = (char*) calloc ( max_cmd_size, sizeof(char) );
    
    while (line != NULL && line_info.cmd_line_num < assmblr->file_lines_num) {
        
        if (line[0] != '\0') { //TODO Переписать с поэтапным scanf %s
            
            line_info.elements = sscanf ( (const char*)line, "%s %s", line_info.cmd, line_info.arg ); 

            status = HandleNonCmdCases ( &line_info, assmblr );
            PROC_STATUS_CHECK

            if ( (line_info.is_cmd == 1) && (line_info.is_comment == 0) ) {

                status = CmdConvToCode ( assmblr, &line_info );
                PROC_STATUS_CHECK

                assmblr->cmd_buffer[assmblr->cmd_num] = line_info.cmd_code;
                assmblr->cmd_num++;

                if ( line_info.has_arg ) {
                    status = ArgConvToCode ( assmblr, &line_info );
                    PROC_STATUS_CHECK

                    assmblr->cmd_buffer[assmblr->cmd_num] = line_info.arg_code;
                    assmblr->cmd_num++;
                }

            }

        }
        
        line = strtok(NULL, "\n");
    }

    return status;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t HandleNonCmdCases ( Cmd_Line_t* line_info, Cmd_Assemblr_t* assmblr ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    status = ProcessCmdToken ( line_info, assmblr );
    PROC_STATUS_CHECK

    if ( line_info->elements > 1 ) {
        status = ProcessArgToken ( line_info, assmblr );
        PROC_STATUS_CHECK
    }

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

        line_info->elements--;
        return Proc_Err_t::PRC_SUCCSESFUL;

    }

    return Proc_Err_t::PRC_SUCCSESFUL;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t AsmblrPrintFile ( Cmd_Assemblr_t* assmblr, FILE* stream ) {

    int ind = 0;

    while ( ind < assmblr->cmd_num ) {

        fprintf ( stream, "%ld ", (long)assmblr->cmd_buffer[ind] );
        ind++;

    }

    return Proc_Err_t::PRC_SUCCSESFUL;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t CmdConvToCode ( Cmd_Assemblr_t* assmblr, Cmd_Line_t* line_info ) {

    Proc_Err_t status = Proc_Err_t::PRC_SUCCSESFUL;

    int cur_hash = djb2hash ( line_info->cmd );
    int cmd_ind = HashBinSearch ( assmblr->instr_sort_hash, assmblr->cmd_instr_num, cur_hash );
    if ( cmd_ind == -1 ) return Proc_Err_t::UNDEF_COMAND_ERR;

    line_info->cmd_code = assmblr->instr_sort_hash[cmd_ind].cmd_code;

    if ( line_info->elements > 1 )
        status = ResolveCmdCode ( line_info );

    line_info->has_arg  = assmblr->instr_sort_code[line_info->cmd_code].is_arg;

    if ( line_info->cmd_code <= assmblr->def_cmd_num )
        if ( (  assmblr->instr_sort_hash[cmd_ind].is_arg && line_info->elements < 2 ) ||
             ( !assmblr->instr_sort_hash[cmd_ind].is_arg && line_info->elements > 1 ))
                return Proc_Err_t::INCOR_ARG_NUM_ERR;

    return status;

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

    return Proc_Err_t::PRC_SUCCSESFUL;

}

/*-----------------------------------------------------------------------------------------------*/

Proc_Err_t ResolveCmdCode ( Cmd_Line_t* line_info ) {

    if ( line_info->arg[0] == 'R' && line_info->arg[2] == 'X' ) {

        if ( line_info->cmd_code < 32 ) line_info->cmd_code += 32;
        else if ( line_info->cmd_code > 64 ) return Proc_Err_t::INCOR_ARG_ERR;
    
    }

    if ( line_info->arg[0] == '[' && line_info->arg[4] == ']' &&
         line_info->arg[1] == 'R' && line_info->arg[3] == 'X' ) {

        if ( line_info->cmd_code < 32 ) line_info->cmd_code += 64;
        else if ( line_info->cmd_code < 64 ) return Proc_Err_t::INCOR_ARG_ERR;

    }

    return Proc_Err_t::PRC_SUCCSESFUL;
    
}

/*-----------------------------------------------------------------------------------------------*/


